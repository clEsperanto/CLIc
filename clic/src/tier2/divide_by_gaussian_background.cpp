#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

namespace cle::tier2
{

auto
divide_by_gaussian_background_func(const Device::Pointer & device,
                                   const Array::Pointer &  src,
                                   Array::Pointer          dst,
                                   float                   sigma_x,
                                   float                   sigma_y,
                                   float                   sigma_z) -> Array::Pointer
{
  auto temp = tier1::gaussian_blur_func(device, src, nullptr, sigma_x, sigma_y, sigma_z);
  return tier1::divide_images_func(device, src, temp, dst);
}

} // namespace cle::tier2
