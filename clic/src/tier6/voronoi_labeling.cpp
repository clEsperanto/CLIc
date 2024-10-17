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
voronoi_labeling_func(const Device::Pointer & device, const Array::Pointer & input_binary, Array::Pointer output_labels)
  -> Array::Pointer
{
  tier0::create_like(input_binary, output_labels, dType::LABEL);
  auto flip = tier5::connected_component_labeling_func(device, input_binary, nullptr, "box");
  return tier2::extend_labeling_via_voronoi_func(device, flip, output_labels);
}

} // namespace cle::tier6
