#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

namespace cle::tier1
{

namespace
{

// Native 1D kernel template.
constexpr const char * kernel_template = R"SRC(
{KW} void math_unary(
  {ADDR}const {SRC_T}* src,
  {ADDR}{DST_T}*       dst,
  const int            size
) {
  const int idx = {GID};
  if (idx >= size) return;
  const float x = (float)src[idx];
  dst[idx] = ({DST_T})({OP});
}
)SRC";

auto
apply_unary_math_operation(const Device::Pointer & device,
                           const Array::Pointer &  src,
                           Array::Pointer          dst,
                           const std::string &     op_expr) -> Array::Pointer
{
  tier0::create_like(src, dst);

  const bool        is_opencl = (device->getType() == Device::Type::OPENCL);
  const std::string src_type = toString(src->dtype());
  const std::string dst_type = toString(dst->dtype());

  const std::string source = renderTemplate(kernel_template, {
    { "KW",    is_opencl ? "__kernel" : "extern \"C\" __global__" },
    { "ADDR",  is_opencl ? "__global " : "" },
    { "SRC_T", src_type },
    { "DST_T", dst_type },
    { "GID",   is_opencl ? "get_global_id(0)" : "blockDim.x * blockIdx.x + threadIdx.x" },
    { "OP",    op_expr }
  });

  const int        total_size = static_cast<int>(src->size());
  const size_t     max_local = device->getMaximumWorkGroupSize();
  const size_t     global_padded = ((total_size + max_local - 1) / max_local) * max_local;
  const RangeArray global_range = { global_padded, 1, 1 };
  const RangeArray local_range = { max_local, 1, 1 };

  const KernelInfo    kernel = { "math_unary", source };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "size", total_size } };
  native_execute(device, kernel, params, global_range, local_range);
  return dst;
}

} // namespace

auto
absolute_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_unary_math_operation(device, src, dst, "fabs(x)");
}

auto
cubic_root_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_unary_math_operation(device, src, dst, "cbrt(x)");
}

auto
square_root_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_unary_math_operation(device, src, dst, "sqrt(x)");
}

auto
exponential_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_unary_math_operation(device, src, dst, "exp(x)");
}

auto
exponential2_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_unary_math_operation(device, src, dst, "exp2(x)");
}

auto
exponential10_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_unary_math_operation(device, src, dst, "exp10(x)");
}

auto
logarithm_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_unary_math_operation(device, src, dst, "log(x)");
}

auto
logarithm2_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_unary_math_operation(device, src, dst, "log2(x)");
}

auto
logarithm10_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_unary_math_operation(device, src, dst, "log10(x)");
}

auto
reciprocal_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_unary_math_operation(device, src, dst, "1.0f / x");
}

auto
ceil_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_unary_math_operation(device, src, dst, "ceil(x)");
}

auto
floor_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_unary_math_operation(device, src, dst, "floor(x)");
}

auto
round_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_unary_math_operation(device, src, dst, "round(x)");
}

auto
truncate_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_unary_math_operation(device, src, dst, "trunc(x)");
}

auto
binary_not_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_unary_math_operation(device, src, dst, "(x != 0) ? 0 : 1");
}


} // namespace cle::tier1
