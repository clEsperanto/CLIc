#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"

#include "statistics.hpp"
#include "utils.hpp"
#include <numeric>

namespace cle::tier5
{

auto
filter_label_by_size_func(const Device::Pointer & device,
                          const Array::Pointer &  src,
                          Array::Pointer          dst,
                          float                   minimum_size,
                          float                   maximum_size) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::LABEL);
  // Only the pixel count per label is needed: a single accumulation pass on the labels, without the background (it is
  // always kept, whatever its size) nor the other statistics.
  const auto nb_labels = static_cast<size_t>(tier1::maximum_of_all_pixels_func(device, src)) + 1;
  auto       stats = _statistics_per_label(device, src, src, static_cast<int>(nb_labels));
  auto       list_of_area = Array::create(nb_labels, 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  stats->copyTo(list_of_area, { nb_labels, 1, 1 }, { 0, 3, 0 }, { 0, 0, 0 }); // plane 3: pixel count

  return tier4::remove_labels_with_map_values_out_of_range_func(device, src, list_of_area, dst, minimum_size, maximum_size);
}

auto
exclude_labels_outside_size_range_func(const Device::Pointer & device,
                                       const Array::Pointer &  src,
                                       Array::Pointer          dst,
                                       float                   minimum_size,
                                       float                   maximum_size) -> Array::Pointer
{
  return filter_label_by_size_func(device, src, dst, minimum_size, maximum_size);
}

} // namespace cle::tier5
