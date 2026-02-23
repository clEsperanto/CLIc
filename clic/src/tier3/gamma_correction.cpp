#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"

#include "utils.hpp"

namespace cle::tier3
{

auto
gamma_correction_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float gamma) -> Array::Pointer
{
  tier0::create_like(src, dst);
  auto max_intensity = tier2::maximum_of_all_pixels_func(device, src);
  evaluate(device, "pow(a / m, g) * m", {src, max_intensity, gamma}, dst);
  return dst;
}

} // namespace cle::tier3
