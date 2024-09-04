#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"

#include "utils.hpp"
#include <numeric>

namespace cle::tier4
{

auto
filter_label_by_size_func(const Device::Pointer & device,
                          const Array::Pointer &  src,
                          Array::Pointer          dst,
                          float                   min_size,
                          float                   max_size) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::LABEL);
  auto stats = tier3::statistics_of_labelled_pixels_func(device, src, nullptr, true);

  const auto nb_labels = stats["label"].size();
  auto       list_of_area = Array::create(nb_labels, 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  list_of_area->writeFrom(stats["area"].data());

  auto below_min_buffer = tier1::smaller_constant_func(device, list_of_area, nullptr, min_size);
  auto above_max_buffer = tier1::greater_constant_func(device, list_of_area, nullptr, max_size);

  auto list_of_labels_to_exclude = Array::create(nb_labels, 1, 1, 1, dType::LABEL, mType::BUFFER, device);
  tier1::binary_or_func(device, below_min_buffer, above_max_buffer, list_of_labels_to_exclude);
  return tier3::remove_labels_func(device, src, list_of_labels_to_exclude, dst);
}


} // namespace cle::tier4
