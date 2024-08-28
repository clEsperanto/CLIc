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
remove_holes_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float max_size)
  -> Array::Pointer
{
  // get the holes by looking at the inverted image
  auto binary = tier1::greater_constant_func(device, src, nullptr, 0);
  auto inverted = tier1::binary_not_func(device, binary, nullptr);
  auto labels = tier5::connected_components_labeling_func(device, inverted, nullptr, "box");
  tier5::remove_small_objects_func(device, labels, dst, max_size);
  // invert the filtered image
  tier1::greater_constant_func(device, dst, binary);
  teir1::binary_not_func(device, binary, inverted);
  return tier1::multiply_images_func(device, src, inverted, dst);
}


} // namespace cle::tier6
