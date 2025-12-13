#include "tier0.hpp"
#include "tier1.hpp"

#include "cle_unary_operation.h"
#include "utils.hpp"

namespace cle::tier1
{

namespace
{
auto
apply_unary_math_operation(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, const std::string & op_define)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel_info = { "unary_operation", kernel::unary_operation };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  const ConstantList  constants = { { "APPLY_OP(x)", op_define } };
  execute(device, kernel_info, params, range, { 0, 0, 0 }, constants);
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
  return apply_unary_math_operation(device, src, dst, "native_sqrt(x)");
}

auto
exponential_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_unary_math_operation(device, src, dst, "native_exp(x)");
}

auto
exponential2_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_unary_math_operation(device, src, dst, "native_exp2(x)");
}

auto
exponential10_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_unary_math_operation(device, src, dst, "native_exp10(x)");
}

auto
logarithm_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_unary_math_operation(device, src, dst, "native_log(x)");
}

auto
logarithm2_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_unary_math_operation(device, src, dst, "native_log2(x)");
}

auto
logarithm10_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_unary_math_operation(device, src, dst, "native_log10(x)");
}

auto
reciprocal_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_unary_math_operation(device, src, dst, "native_recip(x)");
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
  return apply_unary_math_operation(device, src, dst, "(x == 0)");
}


} // namespace cle::tier1
