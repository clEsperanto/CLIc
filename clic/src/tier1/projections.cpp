#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_z_position_projection.h"

namespace kernel
{

// Generalized variance / standard-deviation projection kernel for any axis
// (np.var / np.std along the target axis). Uses Welford's online algorithm for
// a single-pass, numerically stable computation. Set compute_std to a non-zero
// value to return the standard deviation (square root of the variance).
constexpr const char * deviation_projection_kernel = R"CLC(
__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

__kernel void deviation_projection(
    IMAGE_src_TYPE  src,
    IMAGE_dst_TYPE  dst,
    int axis,       // 0=X projection, 1=Y projection, 2=Z projection
    int ddof,       // delta degrees of freedom (divisor is n - ddof)
    int compute_std // 0=variance, non-zero=standard deviation
)
{
  const int id0 = get_global_id(0);
  const int id1 = get_global_id(1);

  // Projection length along the target axis
  const int n = (axis == 0) ? GET_IMAGE_WIDTH(src) :
                (axis == 1) ? GET_IMAGE_HEIGHT(src) : GET_IMAGE_DEPTH(src);

  // Welford's online algorithm: single pass, numerically stable
  float mean = 0;
  float m2 = 0;

  for (int i = 0; i < n; i++)
  {
    // Map (id0, id1, i) to (x, y, z) based on projection axis
    const int x = (axis == 0) ? i   : id0;
    const int y = (axis == 0) ? id0 : (axis == 1) ? i : id1;
    const int z = (axis == 2) ? i   : id1;

    const float value = (float) READ_IMAGE(src, sampler, POS_src_INSTANCE(x, y, z, 0)).x;
    const float delta = value - mean;
    mean += delta / (float)(i + 1);
    const float delta2 = value - mean;
    m2 += delta * delta2;
  }

  const int   dof = n - ddof;
  float       result = (dof > 0) ? (m2 / (float)dof) : 0;
  if (compute_std)
  {
    result = sqrt(result);
  }

  WRITE_IMAGE(dst, POS_dst_INSTANCE(id0, id1, 0, 0), CONVERT_dst_PIXEL_TYPE(result));
}
)CLC";

// Projection along one axis (0 = x, 1 = y, 2 = z), one work-item per output pixel. The operation is selected with the
// PROJECTION_OPERATION constant: 0 = sum, 1 = mean, 2 = product (accumulated in float), 3 = minimum, 4 = maximum
// (in the source type).
constexpr const char * axis_projection_kernel = R"CLC(
__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

__kernel void axis_projection(
    IMAGE_src_TYPE  src,
    IMAGE_dst_TYPE  dst,
    int axis  // 0=X projection, 1=Y projection, 2=Z projection
)
{
  const int id0 = get_global_id(0);
  const int id1 = get_global_id(1);

  // Projection length along the target axis
  const int n = (axis == 0) ? GET_IMAGE_WIDTH(src) :
                (axis == 1) ? GET_IMAGE_HEIGHT(src) : GET_IMAGE_DEPTH(src);

#if PROJECTION_OPERATION <= 2
  float result = (PROJECTION_OPERATION == 2) ? 1 : 0;
#else
  IMAGE_src_PIXEL_TYPE result = 0;
#endif
  for (int i = 0; i < n; i++)
  {
    // Map (id0, id1, i) to (x, y, z) based on projection axis
    const int x = (axis == 0) ? i   : id0;
    const int y = (axis == 0) ? id0 : (axis == 1) ? i : id1;
    const int z = (axis == 2) ? i   : id1;

#if PROJECTION_OPERATION <= 2
    const float value = (float) READ_IMAGE(src, sampler, POS_src_INSTANCE(x, y, z, 0)).x;
#else
    const IMAGE_src_PIXEL_TYPE value = READ_IMAGE(src, sampler, POS_src_INSTANCE(x, y, z, 0)).x;
#endif
#if PROJECTION_OPERATION <= 1
    result += value;
#elif PROJECTION_OPERATION == 2
    result *= value;
#elif PROJECTION_OPERATION == 3
    result = (i == 0 || value < result) ? value : result;
#else
    result = (i == 0 || value > result) ? value : result;
#endif
  }
#if PROJECTION_OPERATION == 1
  result = result / (float) n;
#endif

  WRITE_IMAGE(dst, POS_dst_INSTANCE(id0, id1, 0, 0), CONVERT_dst_PIXEL_TYPE(result));
}
)CLC";

// Position of the minimum or maximum along one axis (0 = x, 1 = y, 2 = z): the first position where it is reached.
// POSITION_OPERATION constant: 0 = minimum, 1 = maximum.
constexpr const char * axis_position_projection_kernel = R"CLC(
__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

__kernel void axis_position_projection(
    IMAGE_src_TYPE  src,
    IMAGE_dst_TYPE  dst,
    int axis  // 0=X projection, 1=Y projection, 2=Z projection
)
{
  const int id0 = get_global_id(0);
  const int id1 = get_global_id(1);

  // Projection length along the target axis
  const int n = (axis == 0) ? GET_IMAGE_WIDTH(src) :
                (axis == 1) ? GET_IMAGE_HEIGHT(src) : GET_IMAGE_DEPTH(src);

  float best = 0;
  int   best_position = 0;
  for (int i = 0; i < n; i++)
  {
    // Map (id0, id1, i) to (x, y, z) based on projection axis
    const int x = (axis == 0) ? i   : id0;
    const int y = (axis == 0) ? id0 : (axis == 1) ? i : id1;
    const int z = (axis == 2) ? i   : id1;

    const float value = READ_IMAGE(src, sampler, POS_src_INSTANCE(x, y, z, 0)).x;
#if POSITION_OPERATION == 1
    if (value > best || i == 0)
#else
    if (value < best || i == 0)
#endif
    {
      best = value;
      best_position = i;
    }
  }
  WRITE_IMAGE(dst, POS_dst_INSTANCE(id0, id1, 0, 0), CONVERT_dst_PIXEL_TYPE(best_position));
}
)CLC";

} // namespace kernel

namespace cle::tier1
{

namespace
{
auto
keepdims_y(const Array::Pointer & src, const Array::Pointer & dst) -> Array::Pointer
{
  return dst->reshape(dst->width(), 1, dst->height(), src->dimension());
}
auto
keepdims_x(const Array::Pointer & src, const Array::Pointer & dst) -> Array::Pointer
{
  return dst->reshape(1, dst->width(), dst->height(), src->dimension());
}

// Select the accumulator dtype for the sum / product projections based on the
// source dtype: floating-point (or complex) sources accumulate into FLOAT,
// signed integers into INT32, and unsigned integers into UINT32.
auto
accumulator_type(const Array::Pointer & src) -> dType
{
  switch (src->dtype())
  {
    case dType::INT8:
    case dType::INT16:
    case dType::INT32:
      return dType::INT32;
    case dType::UINT8:
    case dType::UINT16:
    case dType::UINT32:
      return dType::UINT32;
    default:
      return dType::FLOAT;
  }
}

enum class Projection : int
{
  SUM = 0,
  MEAN = 1,
  PRODUCT = 2,
  MINIMUM = 3,
  MAXIMUM = 4
};

// Shared implementation of the projections along a given axis (0=X, 1=Y, 2=Z): allocates dst as the source without
// that axis, runs the kernel (arguments: src, dst, axis, then the scalars) and keeps the reduced axis as a singleton
// dimension if requested.
auto
axis_projection(const Device::Pointer & device,
                const Array::Pointer &  src,
                Array::Pointer          dst,
                int                     axis,
                bool                    keep_dims,
                const KernelInfo &      kernel,
                dType                   type,
                const ConstantList &    constants,
                const ParameterList &   scalars = {}) -> Array::Pointer
{
  switch (axis)
  {
    case 0:
      tier0::create_yz(src, dst, type, keep_dims);
      break;
    case 1:
      tier0::create_xz(src, dst, type, keep_dims);
      break;
    default:
      tier0::create_xy(src, dst, type, keep_dims);
      break;
  }
  ParameterList params = { { "src", src }, { "dst", dst }, { "axis", axis } };
  params.insert(params.end(), scalars.begin(), scalars.end());
  const RangeArray range = { dst->width(), dst->height(), 1 };
  execute(device, kernel, params, range, { 0, 0, 0 }, constants);
  if (keep_dims)
  {
    dst = (axis == 0) ? keepdims_x(src, dst) : (axis == 1) ? keepdims_y(src, dst) : dst;
  }
  return dst;
}

// Sum, mean, product, minimum or maximum projection
auto
reduce_projection(const Device::Pointer & device,
                  const Array::Pointer &  src,
                  Array::Pointer          dst,
                  int                     axis,
                  bool                    keep_dims,
                  Projection              operation) -> Array::Pointer
{
  const dType type = (operation == Projection::MEAN)                                      ? dType::FLOAT
                     : (operation == Projection::SUM || operation == Projection::PRODUCT) ? accumulator_type(src)
                                                                                          : dType::UNKNOWN;
  return axis_projection(device,
                         src,
                         dst,
                         axis,
                         keep_dims,
                         { "axis_projection", kernel::axis_projection_kernel },
                         type,
                         { { "PROJECTION_OPERATION", static_cast<int>(operation) } });
}

// Position of the maximum (or minimum) along the axis
auto
position_projection(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int axis, bool keep_dims, bool maximum)
  -> Array::Pointer
{
  return axis_projection(device,
                         src,
                         dst,
                         axis,
                         keep_dims,
                         { "axis_position_projection", kernel::axis_position_projection_kernel },
                         dType::INDEX,
                         { { "POSITION_OPERATION", maximum ? 1 : 0 } });
}

// Variance / standard-deviation projection. Set compute_std to return the standard deviation.
auto
deviation_projection(const Device::Pointer & device,
                     const Array::Pointer &  src,
                     Array::Pointer          dst,
                     int                     axis,
                     int                     ddof,
                     bool                    keep_dims,
                     bool                    compute_std) -> Array::Pointer
{
  return axis_projection(device,
                         src,
                         dst,
                         axis,
                         keep_dims,
                         { "deviation_projection", kernel::deviation_projection_kernel },
                         dType::FLOAT,
                         {},
                         { { "ddof", ddof }, { "compute_std", compute_std ? 1 : 0 } });
}

} // namespace

auto
std_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int ddof, bool keep_dims)
  -> Array::Pointer
{
  return deviation_projection(device, src, dst, 0, ddof, keep_dims, true);
}

auto
std_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int ddof, bool keep_dims)
  -> Array::Pointer
{
  return deviation_projection(device, src, dst, 1, ddof, keep_dims, true);
}

auto
std_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int ddof, bool keep_dims)
  -> Array::Pointer
{
  return deviation_projection(device, src, dst, 2, ddof, keep_dims, true);
}

auto
variance_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int ddof, bool keep_dims)
  -> Array::Pointer
{
  return deviation_projection(device, src, dst, 0, ddof, keep_dims, false);
}

auto
variance_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int ddof, bool keep_dims)
  -> Array::Pointer
{
  return deviation_projection(device, src, dst, 1, ddof, keep_dims, false);
}

auto
variance_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int ddof, bool keep_dims)
  -> Array::Pointer
{
  return deviation_projection(device, src, dst, 2, ddof, keep_dims, false);
}

auto
maximum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, bool keep_dims) -> Array::Pointer
{
  return reduce_projection(device, src, dst, 0, keep_dims, Projection::MAXIMUM);
}

auto
maximum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, bool keep_dims) -> Array::Pointer
{
  return reduce_projection(device, src, dst, 1, keep_dims, Projection::MAXIMUM);
}

auto
maximum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, bool keep_dims) -> Array::Pointer
{
  return reduce_projection(device, src, dst, 2, keep_dims, Projection::MAXIMUM);
}

auto
mean_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, bool keep_dims) -> Array::Pointer
{
  return reduce_projection(device, src, dst, 0, keep_dims, Projection::MEAN);
}

auto
mean_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, bool keep_dims) -> Array::Pointer
{
  return reduce_projection(device, src, dst, 1, keep_dims, Projection::MEAN);
}

auto
mean_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, bool keep_dims) -> Array::Pointer
{
  return reduce_projection(device, src, dst, 2, keep_dims, Projection::MEAN);
}

auto
minimum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, bool keep_dims) -> Array::Pointer
{
  return reduce_projection(device, src, dst, 0, keep_dims, Projection::MINIMUM);
}

auto
minimum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, bool keep_dims) -> Array::Pointer
{
  return reduce_projection(device, src, dst, 1, keep_dims, Projection::MINIMUM);
}

auto
minimum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, bool keep_dims) -> Array::Pointer
{
  return reduce_projection(device, src, dst, 2, keep_dims, Projection::MINIMUM);
}

auto
sum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, bool keep_dims) -> Array::Pointer
{
  return reduce_projection(device, src, dst, 0, keep_dims, Projection::SUM);
}

auto
sum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, bool keep_dims) -> Array::Pointer
{
  return reduce_projection(device, src, dst, 1, keep_dims, Projection::SUM);
}

auto
sum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, bool keep_dims) -> Array::Pointer
{
  return reduce_projection(device, src, dst, 2, keep_dims, Projection::SUM);
}

auto
product_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, bool keep_dims) -> Array::Pointer
{
  return reduce_projection(device, src, dst, 0, keep_dims, Projection::PRODUCT);
}

auto
product_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, bool keep_dims) -> Array::Pointer
{
  return reduce_projection(device, src, dst, 1, keep_dims, Projection::PRODUCT);
}

auto
product_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, bool keep_dims) -> Array::Pointer
{
  return reduce_projection(device, src, dst, 2, keep_dims, Projection::PRODUCT);
}

auto
x_position_of_maximum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, bool keep_dims)
  -> Array::Pointer
{
  return position_projection(device, src, dst, 0, keep_dims, true);
}

auto
x_position_of_minimum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, bool keep_dims)
  -> Array::Pointer
{
  return position_projection(device, src, dst, 0, keep_dims, false);
}

auto
y_position_of_maximum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, bool keep_dims)
  -> Array::Pointer
{
  return position_projection(device, src, dst, 1, keep_dims, true);
}

auto
y_position_of_minimum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, bool keep_dims)
  -> Array::Pointer
{
  return position_projection(device, src, dst, 1, keep_dims, false);
}

auto
z_position_of_maximum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, bool keep_dims)
  -> Array::Pointer
{
  return position_projection(device, src, dst, 2, keep_dims, true);
}

auto
z_position_of_minimum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, bool keep_dims)
  -> Array::Pointer
{
  return position_projection(device, src, dst, 2, keep_dims, false);
}

auto
z_position_projection_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & position, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_xy(src, dst);
  const KernelInfo    kernel = { "z_position_projection", kernel::z_position_projection };
  const ParameterList params = { { "src", src }, { "position", position }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
