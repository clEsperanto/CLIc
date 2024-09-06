#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"
#include "tier6.hpp"

#include "utils.hpp"

namespace cle::tier6
{

auto
voronoi_labeling_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::LABEL);
  auto flip = tier5::connected_component_labeling_func(device, src, nullptr, "box");
  return tier2::extend_labeling_via_voronoi_func(device, flip, dst);
}

} // namespace cle::tier6
