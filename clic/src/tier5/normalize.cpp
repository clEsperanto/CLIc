#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"

#include "utils.hpp"

namespace cle::tier5
{

auto
normalize_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float low_percentile, float high_percentile)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);

  const auto new_min = 0.0f;
  const auto new_max = 1.0f;
  const auto min =
    (low_percentile > 0) ? roundf(tier4::percentile_func(device, src, low_percentile)) : tier2::minimum_of_all_pixels_func(device, src);
  const auto max =
    (high_percentile > 0) ? roundf(tier4::percentile_func(device, src, high_percentile)) : tier2::maximum_of_all_pixels_func(device, src);
  const auto constant = -(new_max - new_min) / (max - min);
  evaluate(device, "fmin(fmax((lo - a) * c, 0.0f), 1.0f)", { min, src, constant }, dst);
  return dst;
}

} // namespace cle::tier5
