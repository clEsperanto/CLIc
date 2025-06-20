
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
pixel_count_map_func(const Device::Pointer & device, const Array::Pointer & input_labels, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_like(input_labels, dst, dType::FLOAT);
  auto props = tier3::statistics_of_background_and_labelled_pixels_func(device, input_labels, input_labels);

  auto values = cle::Array::create(props["area"].size(), 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  values->writeFrom(props["area"].data());

  tier1::set_column_func(device, values, 0, 0);
  return tier1::replace_values_func(device, input_labels, values, dst);
}

auto
label_pixel_count_map_func(const Device::Pointer & device, const Array::Pointer & input_labels, Array::Pointer dst)
  -> Array::Pointer
{
  return pixel_count_map_func(device, input_labels, dst);
}

auto
extension_ratio_map_func(const Device::Pointer & device, const Array::Pointer & input_labels, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_like(input_labels, dst, dType::FLOAT);
  auto props = tier3::statistics_of_background_and_labelled_pixels_func(device, input_labels, input_labels);
  auto vector = props["mean_max_distance_to_centroid_ratio"];
  auto values = Array::create(vector.size(), 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  values->writeFrom(vector.data());
  tier1::set_column_func(device, values, 0, 0);
  return tier1::replace_values_func(device, input_labels, values, dst);
}

auto
mean_extension_map_func(const Device::Pointer & device, const Array::Pointer & input_labels, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_like(input_labels, dst, dType::FLOAT);
  auto props = tier3::statistics_of_background_and_labelled_pixels_func(device, input_labels, input_labels);
  auto vector = props["mean_distance_to_centroid"];
  auto values = Array::create(vector.size(), 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  values->writeFrom(vector.data());
  tier1::set_column_func(device, values, 0, 0);
  return tier1::replace_values_func(device, input_labels, values, dst);
}

auto
maximum_extension_map_func(const Device::Pointer & device, const Array::Pointer & input_labels, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_like(input_labels, dst, dType::FLOAT);
  auto props = tier3::statistics_of_background_and_labelled_pixels_func(device, input_labels, input_labels);
  auto vector = props["max_distance_to_centroid"];
  auto values = Array::create(vector.size(), 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  values->writeFrom(vector.data());
  tier1::set_column_func(device, values, 0, 0);
  return tier1::replace_values_func(device, input_labels, values, dst);
}


auto
mean_intensity_map_func(const Device::Pointer & device,
                        const Array::Pointer &  src,
                        const Array::Pointer &  input_labels,
                        Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  auto props = tier3::statistics_of_background_and_labelled_pixels_func(device, src, input_labels);

  auto values = cle::Array::create(props["mean_intensity"].size(), 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  values->writeFrom(props["mean_intensity"].data());

  tier1::set_column_func(device, values, 0, 0);
  return tier1::replace_values_func(device, input_labels, values, dst);
}

auto
minimum_intensity_map_func(const Device::Pointer & device,
                           const Array::Pointer &  src,
                           const Array::Pointer &  input_labels,
                           Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  auto props = tier3::statistics_of_background_and_labelled_pixels_func(device, src, input_labels);

  auto values = cle::Array::create(props["min_intensity"].size(), 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  values->writeFrom(props["min_intensity"].data());

  tier1::set_column_func(device, values, 0, 0);
  return tier1::replace_values_func(device, input_labels, values, dst);
}

auto
maximum_intensity_map_func(const Device::Pointer & device,
                           const Array::Pointer &  src,
                           const Array::Pointer &  input_labels,
                           Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  auto props = tier3::statistics_of_background_and_labelled_pixels_func(device, src, input_labels);

  auto values = cle::Array::create(props["max_intensity"].size(), 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  values->writeFrom(props["max_intensity"].data());

  tier1::set_column_func(device, values, 0, 0);
  return tier1::replace_values_func(device, input_labels, values, dst);
}

auto
standard_deviation_intensity_map_func(const Device::Pointer & device,
                                      const Array::Pointer &  src,
                                      const Array::Pointer &  input_labels,
                                      Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  auto props = tier3::statistics_of_background_and_labelled_pixels_func(device, src, input_labels);

  auto values = cle::Array::create(props["std_intensity"].size(), 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  values->writeFrom(props["std_intensity"].data());

  tier1::set_column_func(device, values, 0, 0);
  return tier1::replace_values_func(device, input_labels, values, dst);
}


auto
touching_neighbor_count_map_func(const Device::Pointer & device,
                                 const Array::Pointer &  input_labels,
                                 Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(input_labels, dst, dType::FLOAT);
  auto touch_matrix = tier3::generate_touch_matrix_func(device, input_labels, nullptr);
  tier1::set_column_func(device, touch_matrix, 0, 0);
  auto nb_touching_neighbors = tier2::count_touching_neighbors_func(device, touch_matrix, nullptr, true);
  return tier1::replace_values_func(device, input_labels, nb_touching_neighbors, dst);
}


} // namespace cle::tier4
