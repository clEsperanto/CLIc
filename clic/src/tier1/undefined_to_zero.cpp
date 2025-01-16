#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_undefined_to_zero.h"

namespace cle::tier1
{

auto
undefined_to_zero_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel_code = { "undefined_to_zero", kernel::undefined_to_zero };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel_code, params, range);
  return dst;
}

} // namespace cle::tier1
