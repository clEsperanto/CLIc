


#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"

#include "utils.hpp"
#include <algorithm>
#include <numeric>

namespace cle::tier4
{

namespace
{

// Remove the labels whose value (values[label], compared in float) satisfies `remove`; the background is always kept.
// The value list holds one value per label, so it is compared on the host: one read-back instead of three kernel
// launches, and remove_labels_func reads the flags back anyway.
template <typename Predicate>
auto
remove_labels_where(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    const Array::Pointer &  values,
                    Array::Pointer          dst,
                    Predicate               remove) -> Array::Pointer
{
  Array::check_ptr(values, "Error: 'values' is null. Please ensure the provided parameters before calling this function.");
  auto values_float = values;
  if (values->dtype() != dType::FLOAT)
  {
    values_float = Array::create(values->width(), values->height(), values->depth(), values->dim(), dType::FLOAT, mType::BUFFER, device);
    tier1::copy_func(device, values, values_float);
  }
  std::vector<float> host_values(values->size());
  values_float->readTo(host_values.data());

  std::vector<uint32_t> flags(host_values.size());
  std::transform(host_values.begin(), host_values.end(), flags.begin(), [&remove](float v) { return remove(v) ? 1U : 0U; });
  auto flaglist = Array::create(flags.size(), 1, 1, 1, dType::LABEL, mType::BUFFER, device);
  flaglist->writeFrom(flags.data());
  return tier3::remove_labels_func(device, src, flaglist, dst);
}

} // namespace

auto
remove_labels_with_map_values_out_of_range_func(const Device::Pointer & device,
                                                const Array::Pointer &  src,
                                                const Array::Pointer &  values,
                                                Array::Pointer          dst,
                                                float                   min_value,
                                                float                   max_value) -> Array::Pointer
{
  return remove_labels_where(device, src, values, dst, [=](float v) { return v < min_value || v > max_value; });
}


auto
remove_labels_with_map_values_within_range_func(const Device::Pointer & device,
                                                const Array::Pointer &  src,
                                                const Array::Pointer &  values,
                                                Array::Pointer          dst,
                                                float                   min_value,
                                                float                   max_value) -> Array::Pointer
{
  return remove_labels_where(device, src, values, dst, [=](float v) { return v >= min_value && v <= max_value; });
}


auto
exclude_labels_with_map_values_out_of_range_func(const Device::Pointer & device,
                                                 const Array::Pointer &  values_map,
                                                 const Array::Pointer &  label_map_input,
                                                 Array::Pointer          dst,
                                                 float                   minimum_value_range,
                                                 float                   maximum_value_range) -> Array::Pointer
{
  return remove_labels_with_map_values_out_of_range_func(
    device, label_map_input, values_map, dst, minimum_value_range, maximum_value_range);
}


auto
exclude_labels_with_map_values_within_range_func(const Device::Pointer & device,
                                                 const Array::Pointer &  values_map,
                                                 const Array::Pointer &  label_map_input,
                                                 Array::Pointer          dst,
                                                 float                   minimum_value_range,
                                                 float                   maximum_value_range) -> Array::Pointer
{
  return remove_labels_with_map_values_within_range_func(
    device, label_map_input, values_map, dst, minimum_value_range, maximum_value_range);
}

} // namespace cle::tier4
