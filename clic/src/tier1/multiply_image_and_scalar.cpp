#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_multiply_image_and_scalar.h"

namespace cle::tier1
{

auto
multiply_image_and_scalar_func(const Device::Pointer & device,
                               const Array::Pointer &  src,
                               Array::Pointer          dst,
                               float                   scalar) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel_code = { "multiply_image_and_scalar", kernel::multiply_image_and_scalar };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar", scalar } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel_code, params, range);
  return dst;
}

} // namespace cle::tier1
