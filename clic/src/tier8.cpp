
#include "tier8.hpp"
#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"
#include "tier6.hpp"
#include "tier7.hpp"

namespace cle::tier8
{

auto
smooth_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::LABEL);
  if (radius < 1)
  {
    return tier1::copy_func(device, src, dst);
  }
  auto binary = tier1::greater_constant_func(device, src, nullptr, 0);
  auto opened = tier7::opening_labels_func(device, src, nullptr, radius);
  auto extended = tier2::extend_labeling_via_voronoi_func(device, opened, nullptr);
  return tier1::multiply_images_func(device, binary, extended, dst);
}


auto
smooth_connected_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::LABEL);
  if (radius < 1)
  {
    return tier1::copy_func(device, src, dst);
  }
  auto binary = tier7::erode_connected_labels_func(device, src, nullptr, radius);
  return tier6::dilate_labels_func(device, binary, dst, radius);
}

} // namespace cle::tier8
