#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

namespace cle::tier2
{

auto
standard_deviation_func(const Device::Pointer & device,
                        const Array::Pointer &  src,
                        Array::Pointer          dst,
                        int                     radius_x,
                        int                     radius_y,
                        int                     radius_z,
                        std::string             connectivity) -> Array::Pointer
{
  auto temp = tier1::variance_func(device, src, nullptr, radius_x, radius_y, radius_z, connectivity);
  return tier1::power_func(device, temp, dst, 0.5);
}

} // namespace cle::tier2
