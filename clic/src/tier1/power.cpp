#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_power.h"
#include "cle_power_images.h"

namespace cle::tier1
{

auto
power_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "power", kernel::power };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar", scalar } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
