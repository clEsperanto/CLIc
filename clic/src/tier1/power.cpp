#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_power.h"
#include "cle_power_images.h"

namespace cle::tier1
{

auto
power_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float exponent)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  const KernelInfo    kernel_code = { "power", kernel::power };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar", exponent } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel_code, params, range);
  return dst;
}

} // namespace cle::tier1
