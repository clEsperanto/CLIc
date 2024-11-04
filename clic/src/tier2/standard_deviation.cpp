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
                        float                     radius_x,
                        float                     radius_y,
                        float                     radius_z,
                        std::string             connectivity) -> Array::Pointer
{
  auto temp = tier1::variance_filter_func(device, src, nullptr, radius_x, radius_y, radius_z, connectivity);
  return tier1::power_func(device, temp, dst, 0.5);
}

auto
standard_deviation_sphere_func(const Device::Pointer & device,
                               const Array::Pointer &  src,
                               Array::Pointer          dst,
                               float                     radius_x,
                               float                     radius_y,
                               float                     radius_z) -> Array::Pointer
                               {
                                return standard_deviation_func(device, src, dst, radius_x, radius_y, radius_z, "sphere");
                               }


                               auto
standard_deviation_box_func(const Device::Pointer & device,
                            const Array::Pointer &  src,
                            Array::Pointer          dst,
                            float                     radius_x,
                            float                     radius_y,
                            float                     radius_z) -> Array::Pointer
                            {
                              return standard_deviation_func(device, src, dst, radius_x, radius_y, radius_z, "box");
                            }

} // namespace cle::tier2
