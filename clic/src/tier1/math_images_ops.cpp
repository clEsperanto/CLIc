#include "tier0.hpp"
#include "tier1.hpp"
#include "utils.hpp"

#include "cle_add_images_weighted.h"
#include "cle_multiply_image_and_position.h"

namespace cle::tier1
{

namespace
{
// Note: stick to CLIJ macro because it handles broadcasting between images
constexpr const char * kernel_source = R"CLC(
__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

#ifndef APPLY_OP
  #error "APPLY_OP must be defined as a macro (e.g., #define APPLY_OP(x,y) (x+y))"
#endif

__kernel void image_operation(
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
}
)CLC";

auto
apply_images_math_operation(const Device::Pointer & device,
                            const Array::Pointer &  src0,
                            const Array::Pointer &  src1,
                            Array::Pointer          dst,
                            const std::string &     op_define) -> Array::Pointer
{
  const KernelInfo kernel_info = { "image_operation", kernel_source };
  ParameterList    params;
  params.emplace_back("src0", src0);
  params.emplace_back("src1", src1);
  params.emplace_back("dst", dst);
  const RangeArray range = { dst->width(), dst->height(), dst->depth() };
  ConstantList     constants;
  constants.emplace_back("APPLY_OP(x,y)", op_define);
  execute(device, kernel_info, params, range, { 0, 0, 0 }, constants);
  return dst;
}
} // namespace

auto
power_images_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_or_check_broadcast_dst(src0, src1, dst, dType::FLOAT);
  return apply_images_math_operation(device, src0, src1, dst, "pow(x, y)");
}

auto
maximum_images_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_or_check_broadcast_dst(src0, src1, dst, promoteType(src0->dtype(), src1->dtype()));
  return apply_images_math_operation(device, src0, src1, dst, "fmax(x, y)");
}

auto
minimum_images_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_or_check_broadcast_dst(src0, src1, dst, promoteType(src0->dtype(), src1->dtype()));
  return apply_images_math_operation(device, src0, src1, dst, "fmin(x, y)");
}

auto
modulo_images_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_or_check_broadcast_dst(src0, src1, dst, promoteType(src0->dtype(), src1->dtype()));
  return apply_images_math_operation(device, src0, src1, dst, "fmod(x, y)");
}

auto
multiply_images_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_or_check_broadcast_dst(src0, src1, dst, promoteType(src0->dtype(), src1->dtype()));
  return apply_images_math_operation(device, src0, src1, dst, "(x * y)");
}

auto
divide_images_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_or_check_broadcast_dst(src0, src1, dst, dType::FLOAT);
  return apply_images_math_operation(device, src0, src1, dst, "(x / y)");
}

auto
greater_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_or_check_broadcast_dst(src0, src1, dst, dType::BOOL);
  return apply_images_math_operation(device, src0, src1, dst, "(x > y) ? 1 : 0");
}

auto
greater_or_equal_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_or_check_broadcast_dst(src0, src1, dst, dType::BOOL);
  return apply_images_math_operation(device, src0, src1, dst, "(x >= y) ? 1 : 0");
}

auto
smaller_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_or_check_broadcast_dst(src0, src1, dst, dType::BOOL);
  return apply_images_math_operation(device, src0, src1, dst, "(x < y) ? 1 : 0");
}

auto
smaller_or_equal_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_or_check_broadcast_dst(src0, src1, dst, dType::BOOL);
  return apply_images_math_operation(device, src0, src1, dst, "(x <= y) ? 1 : 0");
}

auto
equal_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_or_check_broadcast_dst(src0, src1, dst, dType::BOOL);
  return apply_images_math_operation(device, src0, src1, dst, "(x == y) ? 1 : 0");
}

auto
not_equal_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_or_check_broadcast_dst(src0, src1, dst, dType::BOOL);
  return apply_images_math_operation(device, src0, src1, dst, "(x != y) ? 1 : 0");
}

auto
binary_and_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_or_check_broadcast_dst(src0, src1, dst, dType::BOOL);
  return apply_images_math_operation(device, src0, src1, dst, "(x != 0 && y != 0) ? 1 : 0");
}

auto
binary_or_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_or_check_broadcast_dst(src0, src1, dst, dType::BOOL);
  return apply_images_math_operation(device, src0, src1, dst, "(x != 0 || y != 0) ? 1 : 0");
}

auto
binary_xor_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_or_check_broadcast_dst(src0, src1, dst, dType::BOOL);
  return apply_images_math_operation(device, src0, src1, dst, "((x != 0) != (y != 0)) ? 1 : 0");
}

auto
binary_subtract_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_or_check_broadcast_dst(src0, src1, dst, dType::BOOL);
  return apply_images_math_operation(device, src0, src1, dst, "(x != 0 && y == 0) ? 1 : 0");
}

auto
add_images_weighted_func(const Device::Pointer & device,
                         const Array::Pointer &  src0,
                         const Array::Pointer &  src1,
                         Array::Pointer          dst,
                         float                   factor1,
                         float                   factor2) -> Array::Pointer
{
  tier0::create_or_check_broadcast_dst(src0, src1, dst, promoteType(src0->dtype(), src1->dtype()));
  std::vector<ParameterType> params;
  params.emplace_back(src0);
  params.emplace_back(factor1);
  params.emplace_back(src1);
  params.emplace_back(factor2);
  evaluate(device, "a * x + b * y", params, dst);
  return dst;
}

auto
multiply_image_and_position_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int dimension)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "multiply_image_and_position", kernel::multiply_image_and_position };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "index", dimension } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
