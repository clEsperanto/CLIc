#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

namespace cle::tier2
{

auto
clip_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float min_intensity, float max_intensity)
  -> Array::Pointer
{

  tier0::create_like(src, dst);
  min_intensity = std::isnan(min_intensity) ? tier2::minimum_of_all_pixels_func(device, src) : min_intensity;
  max_intensity = std::isnan(max_intensity) ? tier2::maximum_of_all_pixels_func(device, src) : max_intensity;
  evaluate(device, "clamp(a, lo, hi)", { src, min_intensity, max_intensity }, dst);
  return dst;
}

} // namespace cle::tier2
