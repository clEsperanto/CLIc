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

  __kernel void cle_binary_operation(
      IMAGE_src_TYPE src,
      IMAGE_dst_TYPE dst,
      const float    scalar
  )
  {
    int x = get_global_id(0);
    int y = get_global_id(1);
    int z = get_global_id(2);

    const float value = (float) READ_IMAGE(src, sampler, POS_src_INSTANCE(x,y,z,0)).x;
    const float res = APPLY_OP(value, scalar);

    WRITE_IMAGE(dst, POS_dst_INSTANCE(x,y,z,0), CONVERT_dst_PIXEL_TYPE(res));
  })CLC";

auto
apply_binary_math_operation(const Device::Pointer & device,
                            const Array::Pointer &  src,
                            Array::Pointer          dst,
                            float                   scalar,
                            const std::string &     op_define) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel_info = { "cle_binary_operation", kernel_source };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar", scalar } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  const ConstantList  constants = { { "APPLY_OP(x, y)", op_define } };
  execute(device, kernel_info, params, range, { 1, 1, 1 }, constants);
  return dst;
}

} // namespace

auto
power_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar) -> Array::Pointer
{
  return apply_binary_math_operation(device, src, dst, scalar, "pow(x, y)");
}

auto
root_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar) -> Array::Pointer
{
  return apply_binary_math_operation(device, src, dst, scalar, "rootn(x, y)");
}

auto
maximum_image_and_scalar_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar)
  -> Array::Pointer
{
  return apply_binary_math_operation(device, src, dst, scalar, "fmax(x, y)");
}

auto
minimum_image_and_scalar_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar)
  -> Array::Pointer
{
  return apply_binary_math_operation(device, src, dst, scalar, "fmin(x, y)");
}

auto
add_image_and_scalar_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar) -> Array::Pointer
{
  return apply_binary_math_operation(device, src, dst, scalar, "(x + y)");
}

auto
subtract_scalar_from_image_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar)
  -> Array::Pointer
{
  return apply_binary_math_operation(device, src, dst, scalar, "(x - y)");
}

auto
subtract_image_from_scalar_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar)
  -> Array::Pointer
{
  return apply_binary_math_operation(device, src, dst, scalar, "(y - x)");
}

auto
divide_image_by_scalar_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar) -> Array::Pointer
{
  return apply_binary_math_operation(device, src, dst, scalar, "(x / y)");
}

auto
divide_scalar_by_image_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar) -> Array::Pointer
{
  return apply_binary_math_operation(device, src, dst, scalar, "(y / x)");
}

auto
multiply_image_and_scalar_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar)
  -> Array::Pointer
{
  return apply_binary_math_operation(device, src, dst, scalar, "(x * y)");
}

auto
greater_constant_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar) -> Array::Pointer
{
  return apply_binary_math_operation(device, src, dst, scalar, "(x > y ? 1.0f : 0.0f)");
}

auto
greater_or_equal_constant_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar)
  -> Array::Pointer
{
  return apply_binary_math_operation(device, src, dst, scalar, "(x >= y ? 1.0f : 0.0f)");
}

auto
smaller_constant_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar) -> Array::Pointer
{
  return apply_binary_math_operation(device, src, dst, scalar, "(x < y ? 1.0f : 0.0f)");
}

auto
smaller_or_equal_constant_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar)
  -> Array::Pointer
{
  return apply_binary_math_operation(device, src, dst, scalar, "(x <= y ? 1.0f : 0.0f)");
}

auto
equal_constant_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar) -> Array::Pointer
{
  return apply_binary_math_operation(device, src, dst, scalar, "(x == y ? 1.0f : 0.0f)");
}

auto
not_equal_constant_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar) -> Array::Pointer
{
  return apply_binary_math_operation(device, src, dst, scalar, "(x != y ? 1.0f : 0.0f)");
}


} // namespace cle::tier1
