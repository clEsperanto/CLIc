#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"


#include "cle_maximum_projection.h"
#include "cle_mean_projection.h"
#include "cle_minimum_projection.h"
#include "cle_sum_projection.h"

#include "cle_x_position_of_maximum_x_projection.h"
#include "cle_x_position_of_minimum_x_projection.h"
#include "cle_y_position_of_maximum_y_projection.h"
#include "cle_y_position_of_minimum_y_projection.h"
#include "cle_z_position_of_maximum_z_projection.h"
#include "cle_z_position_of_minimum_z_projection.h"
#include "cle_z_position_projection.h"

namespace kernel
{

// Generalized standard deviation projection kernel for any axis
// Uses Welford's online algorithm for single-pass, numerically stable computation
constexpr const char* std_projection_kernel = R"CLC(
__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

__kernel void std_projection(
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
  
  const float std_value = (n > 1) ? sqrt(m2 / (float)(n - 1)) : 0;
  
  // Output coordinates based on create_* output layout:
  // X projection: create_zy -> (depth, height, 1) range {height, width, 1}
  // Y projection: create_xz -> (width, depth, 1) range {width, depth, 1}
  // Z projection: create_xy -> (width, height, 1) range {width, height, 1}
  const int ox = (axis == 0) ? id1 : id0;
  const int oy = (axis == 0) ? id0 : id1;
  const int oz = 0;
  
  WRITE_IMAGE(dst, POS_dst_INSTANCE(ox, oy, oz, 0), CONVERT_dst_PIXEL_TYPE(std_value));
}
)CLC";

} // namespace kernel

namespace cle::tier1
{

auto
std_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_zy(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "std_projection", kernel::std_projection_kernel };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "axis", 0 } };
  const RangeArray    range = { dst->width(), dst->height(), 1 };
  execute(device, kernel, params, range);
  return dst;
}

auto
std_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xz(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "std_projection", kernel::std_projection_kernel };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "axis", 1 } };
  const RangeArray    range = { dst->width(), dst->height(), 1 };
  execute(device, kernel, params, range);
  return dst;
}

auto
std_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xy(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "std_projection", kernel::std_projection_kernel };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "axis", 2 } };
  const RangeArray    range = { dst->width(), dst->height(), 1 };
  execute(device, kernel, params, range);
  return dst;
}

auto
maximum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_zy(src, dst);
  const KernelInfo    kernel = { "maximum_projection", kernel::maximum_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local = { 0, 0, 0 };
  const ConstantList  constants = { { "PROJECTION_AXIS", 0 } }; // 0 for X axis
  execute(device, kernel, params, range, local, constants);
  return dst;
}

auto
maximum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xz(src, dst);
  const KernelInfo    kernel = { "maximum_projection", kernel::maximum_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local = { 0, 0, 0 };
  const ConstantList  constants = { { "PROJECTION_AXIS", 1 } }; // 1 for Y axis
  execute(device, kernel, params, range, local, constants);
  return dst;
}

auto
maximum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xy(src, dst);
  const KernelInfo    kernel = { "maximum_projection", kernel::maximum_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local = { 0, 0, 0 };
  const ConstantList  constants = { { "PROJECTION_AXIS", 2 } }; // 2 for Z axis
  execute(device, kernel, params, range, local, constants);
  return dst;
}

auto
mean_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_zy(src, dst);
  const KernelInfo    kernel = { "mean_projection", kernel::mean_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local = { 0, 0, 0 };
  const ConstantList  constants = { { "PROJECTION_AXIS", 0 } }; // 0 for X axis
  execute(device, kernel, params, range, local, constants);
  return dst;
}

auto
mean_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xz(src, dst);
  const KernelInfo    kernel = { "mean_projection", kernel::mean_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local = { 0, 0, 0 };
  const ConstantList  constants = { { "PROJECTION_AXIS", 1 } }; // 1 for Y axis
  execute(device, kernel, params, range, local, constants);
  return dst;
}

auto
mean_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xy(src, dst);
  const KernelInfo    kernel = { "mean_projection", kernel::mean_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local = { 0, 0, 0 };
  const ConstantList  constants = { { "PROJECTION_AXIS", 2 } }; // 2 for Z axis
  execute(device, kernel, params, range, local, constants);
  return dst;
}

auto
minimum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_zy(src, dst);
  const KernelInfo    kernel = { "minimum_projection", kernel::minimum_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local = { 0, 0, 0 };
  const ConstantList  constants = { { "PROJECTION_AXIS", 0 } }; // 0 for X axis
  execute(device, kernel, params, range, local, constants);
  return dst;
}
auto
minimum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xz(src, dst);
  const KernelInfo    kernel = { "minimum_projection", kernel::minimum_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local = { 0, 0, 0 };
  const ConstantList  constants = { { "PROJECTION_AXIS", 1 } }; // 1 for Y axis
  execute(device, kernel, params, range, local, constants);
  return dst;
}

auto
minimum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xy(src, dst);
  const KernelInfo    kernel = { "minimum_projection", kernel::minimum_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local = { 0, 0, 0 };
  const ConstantList  constants = { { "PROJECTION_AXIS", 2 } }; // 2 for Z axis
  execute(device, kernel, params, range, local, constants);
  return dst;
}

auto
sum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_zy(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "sum_projection", kernel::sum_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local = { 0, 0, 0 };
  const ConstantList  constants = { { "PROJECTION_AXIS", 0 } }; // 0 for X axis
  execute(device, kernel, params, range, local, constants);
  return dst;
}

auto
sum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xz(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "sum_projection", kernel::sum_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local = { 0, 0, 0 };
  const ConstantList  constants = { { "PROJECTION_AXIS", 1 } }; // 1 for Y axis
  execute(device, kernel, params, range, local, constants);
  return dst;
}

auto
sum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xy(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "sum_projection", kernel::sum_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local = { 0, 0, 0 };
  const ConstantList  constants = { { "PROJECTION_AXIS", 2 } }; // 2 for Z axis
  execute(device, kernel, params, range, local, constants);
  return dst;
}

auto
x_position_of_maximum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_zy(src, dst, dType::INDEX);
  const KernelInfo    kernel = { "x_position_of_maximum_x_projection", kernel::x_position_of_maximum_x_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
x_position_of_minimum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_zy(src, dst, dType::INDEX);
  const KernelInfo    kernel = { "x_position_of_minimum_x_projection", kernel::x_position_of_minimum_x_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
y_position_of_maximum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xz(src, dst, dType::INDEX);
  const KernelInfo    kernel = { "y_position_of_maximum_y_projection", kernel::y_position_of_maximum_y_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
y_position_of_minimum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xz(src, dst, dType::INDEX);
  const KernelInfo    kernel = { "y_position_of_minimum_y_projection", kernel::y_position_of_minimum_y_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
z_position_of_maximum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xy(src, dst, dType::INDEX);
  const KernelInfo    kernel = { "z_position_of_maximum_z_projection", kernel::z_position_of_maximum_z_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
z_position_of_minimum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xy(src, dst, dType::INDEX);
  const KernelInfo    kernel = { "z_position_of_minimum_z_projection", kernel::z_position_of_minimum_z_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
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
