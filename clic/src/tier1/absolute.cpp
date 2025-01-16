#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_absolute.h"

namespace cle::tier1
{

auto
absolute_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel_code = { "absolute", kernel::absolute };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel_code, params, range);
  return dst;
}

} // namespace cle::tier1
