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

  __kernel void cle_image_operation(
      IMAGE_src0_TYPE src0,
      IMAGE_src1_TYPE src1,
      IMAGE_dst_TYPE  dst
  )
  {
    int x = get_global_id(0);
    int y = get_global_id(1);
    int z = get_global_id(2);

    const float value0 = (float) READ_IMAGE(src0, sampler, POS_src0_INSTANCE(x,y,z,0)).x;
    const float value1 = (float) READ_IMAGE(src1, sampler, POS_src1_INSTANCE(x,y,z,0)).x;
    const float res = APPLY_OP(value0, value1);

    WRITE_IMAGE(dst, POS_dst_INSTANCE(x,y,z,0), CONVERT_dst_PIXEL_TYPE(res));
  })CLC";

auto
apply_images_math_operation(const Device::Pointer & device,
                            const Array::Pointer &  src0,
                            const Array::Pointer &  src1,
                            Array::Pointer          dst,
                            const std::string &     op_define) -> Array::Pointer
{
  tier0::create_like(src0, dst);
  const KernelInfo    kernel_info = { "cle_image_operation", kernel_source };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { src0->width(), src0->height(), src0->depth() };
  const ConstantList  constants = { { "APPLY_OP(x,y)", op_define } };
  execute(device, kernel_info, params, range, { 1, 1, 1 }, constants);
  return dst;
}

} // namespace

auto
power_images_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  return apply_images_math_operation(device, src0, src1, dst, "pow(x, y)");
}

auto
maximum_images_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  return apply_images_math_operation(device, src0, src1, dst, "fmax(x, y)");
}

auto
minimum_images_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  return apply_images_math_operation(device, src0, src1, dst, "fmin(x, y)");
}

auto
modulo_images_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  return apply_images_math_operation(device, src0, src1, dst, "fmod(x, y)");
}

auto
multiply_images_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  return apply_images_math_operation(device, src0, src1, dst, "(x * y)");
}

auto
divide_images_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  return apply_images_math_operation(device, src0, src1, dst, "(x / y)");
}

auto
greater_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst) -> Array::Pointer
{
  return apply_images_math_operation(device, src0, src1, dst, "(x > y) ? 1 : 0");
}

auto
greater_or_equal_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  return apply_images_math_operation(device, src0, src1, dst, "(x >= y) ? 1 : 0");
}

auto
smaller_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst) -> Array::Pointer
{
  return apply_images_math_operation(device, src0, src1, dst, "(x < y) ? 1 : 0");
}

auto
smaller_or_equal_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  return apply_images_math_operation(device, src0, src1, dst, "(x <= y) ? 1 : 0");
}

auto
equal_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst) -> Array::Pointer
{
  return apply_images_math_operation(device, src0, src1, dst, "(x == y) ? 1 : 0");
}

auto
not_equal_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  return apply_images_math_operation(device, src0, src1, dst, "(x != y) ? 1 : 0");
}

auto
binary_and_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  return apply_images_math_operation(device, src0, src1, dst, "(x != 0 && y != 0) ? 1 : 0");
}

auto
binary_or_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  return apply_images_math_operation(device, src0, src1, dst, "(x != 0 || y != 0) ? 1 : 0");
}

auto
binary_xor_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  return apply_images_math_operation(device, src0, src1, dst, "((x != 0) != (y != 0)) ? 1 : 0");
}

auto
binary_subtract_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  return apply_images_math_operation(device, src0, src1, dst, "((x != 0) && (y != 0)) ? 0 : 1");
}

} // namespace cle::tier1
