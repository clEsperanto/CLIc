


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
remove_labels_with_map_values_out_of_range_func(const Device::Pointer & device,
                                                const Array::Pointer &  src,
                                                const Array::Pointer &  values,
                                                Array::Pointer          dst,
                                                float                   min_value,
                                                float                   max_value) -> Array::Pointer
{
  auto above = tier1::greater_constant_func(device, values, nullptr, max_value);
  auto below = tier1::smaller_constant_func(device, values, nullptr, min_value);
  auto flaglist = Array::create(values->size(), 1, 1, 1, dType::LABEL, mType::BUFFER, device);
  tier1::binary_or_func(device, below, above, flaglist);
  return tier3::remove_labels_func(device, src, flaglist, dst);
}


auto
remove_labels_with_map_values_within_range_func(const Device::Pointer & device,
                                                const Array::Pointer &  src,
                                                const Array::Pointer &  values,
                                                Array::Pointer          dst,
                                                float                   min_value,
                                                float                   max_value) -> Array::Pointer
{
  auto above = tier1::greater_or_equal_constant_func(device, values, nullptr, max_value);
  auto below = tier1::smaller_or_equal_constant_func(device, values, nullptr, max_value);
  auto flaglist = Array::create(values->size(), 1, 1, 1, dType::LABEL, mType::BUFFER, device);
  tier1::binary_or_func(device, below, above, flaglist);
  return tier3::remove_labels_func(device, src, flaglist, dst);
}


auto
exclude_labels_with_map_values_out_of_range_func(const Device::Pointer & device,
                                                 const Array::Pointer &  values_map,
                                                 const Array::Pointer &  label_map_input,
                                                 Array::Pointer          dst,
                                                 float                   min_value_range,
                                                 float                   max_value_range) -> Array::Pointer
{
  return remove_labels_with_map_values_out_of_range_func(device, label_map_input, values_map, dst, min_value_range, max_value_range);
}


auto
exclude_labels_with_map_values_within_range_func(const Device::Pointer & device,
                                                 const Array::Pointer &  values_map,
                                                 const Array::Pointer &  label_map_input,
                                                 Array::Pointer          dst,
                                                 float                   min_value_range,
                                                 float                   max_value_range) -> Array::Pointer
{
  return remove_labels_with_map_values_within_range_func(device, label_map_input, values_map, dst, min_value_range, max_value_range);
}

} // namespace cle::tier4
