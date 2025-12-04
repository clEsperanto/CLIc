#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"


namespace cle::tier1
{

namespace
{
constexpr const char * kernel_source = R"CLC(

  __constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE |
                                CLK_ADDRESS_CLAMP_TO_EDGE |
                                CLK_FILTER_NEAREST;

  __kernel void cle_trigo_op(
      IMAGE_src_TYPE src,
      IMAGE_dst_TYPE dst
  )
  {
    int x = get_global_id(0);
    int y = get_global_id(1);
    int z = get_global_id(2);

    const float value = (float) READ_IMAGE(src, sampler, POS_src_INSTANCE(x,y,z,0)).x;
    const float res = APPLY_OP(value);

    WRITE_IMAGE(dst, POS_dst_INSTANCE(x,y,z,0), CONVERT_dst_PIXEL_TYPE(res));
  })CLC";

auto
apply_trigonometric_op(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, const std::string & op_define)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel_info = { "cle_trigo_op", kernel_source };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  const ConstantList  constants = { { "APPLY_OP(x)", op_define } };
  execute(device, kernel_info, params, range, { 1, 1, 1 }, constants);
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
