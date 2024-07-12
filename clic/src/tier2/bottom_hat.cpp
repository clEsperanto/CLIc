#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

namespace cle::tier2
{

auto
bottom_hat_func(const Device::Pointer & device,
                const Array::Pointer &  src,
                Array::Pointer          dst,
                float                   radius_x,
                float                   radius_y,
                float                   radius_z,
                std::string             connectivity) -> Array::Pointer
{
  auto temp1 = tier1::maximum_func(device, src, nullptr, radius_x, radius_y, radius_z, connectivity);
  auto temp2 = tier1::minimum_func(device, temp1, nullptr, radius_x, radius_y, radius_z, connectivity);
  return tier1::add_images_weighted_func(device, temp2, src, dst, 1, -1);
}

} // namespace cle::tier2
