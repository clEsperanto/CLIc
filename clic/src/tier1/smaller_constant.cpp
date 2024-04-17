#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_smaller_constant.h"

namespace cle::tier1
{

auto
smaller_constant_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::BINARY);
  const KernelInfo    kernel = { "smaller_constant", kernel::smaller_constant };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar", scalar } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
