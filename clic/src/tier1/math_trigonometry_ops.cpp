#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

namespace cle::tier1
{

namespace
{

const char * kernel_source = R"CLC(
__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

__kernel void math_trigo(
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
apply_trigonometric_op(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, const std::string & op_expr)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  const RangeArray    global_range = { dst->width(), dst->height(), dst->depth() };
  const KernelInfo    kernel = { "math_trigo", kernel_source };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const ConstantList  constants = { { "OP(x)", op_expr } };
  execute(device, kernel, params, global_range, {0,0,0}, constants);
  return dst;
}

} // namespace

auto
sin_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_trigonometric_op(device, src, dst, "sin(x)");
}

auto
cos_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_trigonometric_op(device, src, dst, "cos(x)");
}

auto
tan_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_trigonometric_op(device, src, dst, "tan(x)");
}

auto
asin_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_trigonometric_op(device, src, dst, "asin(x)");
}

auto
acos_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_trigonometric_op(device, src, dst, "acos(x)");
}

auto
atan_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_trigonometric_op(device, src, dst, "atan(x)");
}

auto
sinh_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_trigonometric_op(device, src, dst, "sinh(x)");
}

auto
cosh_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_trigonometric_op(device, src, dst, "cosh(x)");
}

auto
tanh_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_trigonometric_op(device, src, dst, "tanh(x)");
}

} // namespace cle::tier1
