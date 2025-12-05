#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_image_operation.h"

namespace cle::tier1
{

namespace
{
auto
apply_images_math_operation(const Device::Pointer & device,
                            const Array::Pointer &  src0,
                            const Array::Pointer &  src1,
                            Array::Pointer          dst,
                            const std::string &     op_define) -> Array::Pointer
{
  tier0::create_like(src0, dst);
  const KernelInfo    kernel_info = { "image_operation", kernel::image_operation };
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
  return apply_images_math_operation(device, src0, src1, dst, "(x != 0 && y == 0) ? 1 : 0");
}

} // namespace cle::tier1
