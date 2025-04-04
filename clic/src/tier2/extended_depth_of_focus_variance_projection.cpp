#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

namespace cle::tier2
{

auto
extended_depth_of_focus_variance_projection_func(const Device::Pointer & device,
                                                 const Array::Pointer &  src,
                                                 Array::Pointer          dst,
                                                 float                   radius_x,
                                                 float                   radius_y,
                                                 float                   sigma) -> Array::Pointer
{
  auto variance = tier1::variance_filter_func(device, src, nullptr, radius_x, radius_y, 0, "sphere");
  auto temp = tier1::gaussian_blur_func(device, variance, nullptr, sigma, sigma, 0);
  auto altitude = tier1::z_position_of_maximum_z_projection_func(device, temp, nullptr);
  return tier1::z_position_projection_func(device, src, altitude, dst);
}

} // namespace cle::tier2
