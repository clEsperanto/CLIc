#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"

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
  auto stats = tier3::statistics_of_background_and_labelled_pixels_func(device, nullptr, src);

  const auto nb_labels = stats["label"].size();
  auto       list_of_area = Array::create(nb_labels, 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  list_of_area->writeFrom(stats["area"].data());

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
