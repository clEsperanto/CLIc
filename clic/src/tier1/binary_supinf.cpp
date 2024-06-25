#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_superior_inferior.h"

namespace cle::tier1
{

auto
binary_supinf_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "superior_inferior", kernel::superior_inferior };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1