#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

namespace cle::tier2
{

auto
standard_deviation_box_func(const Device::Pointer & device,
                            const Array::Pointer &  src,
                            Array::Pointer          dst,
                            int                     radius_x,
                            int                     radius_y,
                            int                     radius_z) -> Array::Pointer
{
  std::cerr << "Deprecated: this function is deprecated, use standard_deviation instead\n";
  auto temp = tier1::variance_box_func(device, src, nullptr, radius_x, radius_y, radius_z);
  return tier1::power_func(device, temp, dst, 0.5);
}

} // namespace cle::tier2
