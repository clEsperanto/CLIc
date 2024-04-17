#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_binary_not.h"

namespace cle::tier1
{

auto
binary_not_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::BINARY);
  const KernelInfo    kernel = { "binary_not", kernel::binary_not };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
