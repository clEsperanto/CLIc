#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

namespace cle::tier2
{

auto
opening_func(const Device::Pointer & device,
             const Array::Pointer &  src,
             Array::Pointer          dst,
             float                   radius_x,
             float                   radius_y,
             float                   radius_z,
             std::string             connectivity) -> Array::Pointer
{
  auto temp = tier1::minimum_filter_func(device, src, nullptr, radius_x, radius_y, radius_z, connectivity);
  return tier1::maximum_filter_func(device, temp, dst, radius_x, radius_y, radius_z, connectivity);
}

} // namespace cle::tier2
