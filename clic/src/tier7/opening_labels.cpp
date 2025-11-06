#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"
#include "tier6.hpp"
#include "tier7.hpp"

#include "utils.hpp"

namespace cle::tier7
{

auto
opening_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::LABEL);
  auto temp = tier6::erode_labels_func(device, src, nullptr, radius, false);
  return tier6::dilate_labels_func(device, temp, dst, radius);
}

} // namespace cle::tier7
