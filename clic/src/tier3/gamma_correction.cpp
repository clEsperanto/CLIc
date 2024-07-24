#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"

#include "utils.hpp"

namespace cle::tier3
{

auto
gamma_correction_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float gamma)
  -> Array::Pointer
{
  auto max_intensity = tier2::maximum_of_all_pixels_func(device, src);
  auto temp1 = tier1::multiply_image_and_scalar_func(device, src, nullptr, 1.0 / max_intensity);
  auto temp2 = tier1::power_func(device, temp1, nullptr, gamma);
  return tier1::multiply_image_and_scalar_func(device, temp2, dst, max_intensity);
}

} // namespace cle::tier3
