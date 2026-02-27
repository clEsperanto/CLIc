#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

namespace cle::tier1
{

namespace
{

const char * kernel_source = R"CLC(
__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

__kernel void math_unary(
    IMAGE_src_TYPE  src,
    IMAGE_dst_TYPE  dst
) 
{
  const int x = get_global_id(0);
  const int y = get_global_id(1);
  const int z = get_global_id(2);

  const IMAGE_src_PIXEL_TYPE value = READ_IMAGE(src, sampler, POS_src_INSTANCE(x,y,z,0)).x;
  float res = OP(value);
  WRITE_IMAGE(dst, POS_dst_INSTANCE(x,y,z,0), CONVERT_dst_PIXEL_TYPE(res));
})CLC";


auto
apply_unary_math_operation(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, const std::string & op_expr)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  const RangeArray    global_range = { dst->width(), dst->height(), dst->depth() };
  const KernelInfo    kernel = { "math_unary", kernel_source };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const ConstantList  constants = { { "OP(x)", op_expr } };
  execute(device, kernel, params, global_range, {0,0,0}, constants);
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
