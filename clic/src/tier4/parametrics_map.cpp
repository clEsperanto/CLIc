
#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier7.hpp"

#include "utils.hpp"
#include <numeric>

namespace cle::tier4
{

auto
parametric_map_func(const Device::Pointer & device,
                    const Array::Pointer &  labels,
                    const StatisticsMap &   properties,
                    Array::Pointer          dst,
                    const std::string &     target_property) -> Array::Pointer
{
  tier0::create_like(labels, dst, dType::FLOAT);

  // force property name to lower case
  std::string lower_property_name;
  std::transform(target_property.begin(), target_property.end(), std::back_inserter(lower_property_name), ::tolower);

  // Check if property exists
  if (properties.find(lower_property_name) == properties.end())
  {
    throw std::runtime_error("Property '" + target_property + "' not found in statistics");
  }

  auto   nb_labels = tier2::maximum_of_all_pixels_func(device, labels) + 1;
  auto & vector = properties.at(lower_property_name);

  // check if the vector size matches the number of labels
  if (vector.size() != nb_labels)
  {
    throw std::runtime_error("Property '" + target_property + "' has " + std::to_string(vector.size()) +
                             " values, but the label image has " + std::to_string(nb_labels) +
                             " labels. Verify that the statistics were computed on the same image, with include_background at 'True'.");
  }

  auto values = Array::create(vector.size(), 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  values->writeFrom(vector.data());

  tier1::set_column_func(device, values, 0, 0);
  return tier1::replace_values_func(device, labels, values, dst);
}


// -- Label props --

auto
pixel_count_map_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  auto props = tier3::labels_statistics_func(device, src, nullptr, true);
  return parametric_map_func(device, src, props, dst, std::string("area"));
}

auto
label_pixel_count_map_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return pixel_count_map_func(device, src, dst);
}

auto
extension_ratio_map_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  auto props = tier3::labels_statistics_func(device, src, nullptr, true);
  return parametric_map_func(device, src, props, dst, std::string("mean_max_distance_to_centroid_ratio"));
}

auto
mean_extension_map_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  auto props = tier3::labels_statistics_func(device, src, nullptr, true);
  return parametric_map_func(device, src, props, dst, std::string("mean_distance_to_centroid"));
}

auto
maximum_extension_map_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  auto props = tier3::labels_statistics_func(device, src, nullptr, true);
  return parametric_map_func(device, src, props, dst, std::string("max_distance_to_centroid"));
}

// -- Intensity props --

auto
mean_intensity_map_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & labels, Array::Pointer dst)
  -> Array::Pointer
{
  auto props = tier3::labels_statistics_func(device, labels, src, true);
  return parametric_map_func(device, labels, props, dst, std::string("mean_intensity"));
}

auto
label_mean_intensity_map_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & labels, Array::Pointer dst)
  -> Array::Pointer
{
  return mean_intensity_map_func(device, src, labels, dst);
}

auto
minimum_intensity_map_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & labels, Array::Pointer dst)
  -> Array::Pointer
{
  auto props = tier3::labels_statistics_func(device, labels, src, true);
  return parametric_map_func(device, labels, props, dst, std::string("min_intensity"));
}

auto
maximum_intensity_map_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & labels, Array::Pointer dst)
  -> Array::Pointer
{
  auto props = tier3::labels_statistics_func(device, labels, src, true);
  return parametric_map_func(device, labels, props, dst, std::string("max_intensity"));
}

auto
standard_deviation_intensity_map_func(const Device::Pointer & device,
                                      const Array::Pointer &  src,
                                      const Array::Pointer &  labels,
                                      Array::Pointer          dst) -> Array::Pointer
{
  auto props = tier3::labels_statistics_func(device, labels, src, true);
  return parametric_map_func(device, labels, props, dst, std::string("standard_deviation_intensity"));
}


// -- Neighbor props --

// - minimum_touch_portion_dilated_r_10
// - maximum_touch_count_dilated_r_10
// - minimum_touch_count_dilated_r_10
// - maximum_touch_portion_dilated_r_5
// - max_min_distance_ratio_of_touching_neighbors
// - minimum_touch_portion_dilated_r_5
// - minimum_touch_count_dilated_r_5
// - average_distance_of_touching_neighbors_dilated_r_5
// - touching_neighbor_count_dilated_r_10
// - touching_neighbor_count_dilated_r_5
// - minimum_distance_of_touching_neighbors_dilated_r_5
// - maximum_touch_portion
// - average_distance_of_n7_nearest_neighbors
// - average_distance_of_n1_nearest_neighbors
// - average_distance_of_n10_nearest_neighbors
// - maximum_touch_count
// - touch_count_sum_dilated_r_10
// - minimum_distance_of_touching_neighbors
// - proximal_neighbor_count_d10
// - minimum_touch_count
// - touch_portion_above_0.750000_neighbor_count
// - touching_neighbor_count
// - touch_count_sum_dilated_r_5
// - touch_portion_above_0.500000_neighbor_count
// - minimum_touch_portion
// - touch_count_sum
// - maximum_touch_count_dilated_r_5
// - maximum_distance_of_touching_neighbors_dilated_r_5
// - maximum_distance_of_n4_nearest_neighbors
// - touch_portion_above_0.330000_neighbor_count
// - touch_portion_above_0.160000_neighbor_count
// - maximum_distance_of_touching_neighbors_dilated_r_10
// - touch_portion_above_0.000000_neighbor_count
// - distance_to_most_distant_other
// - minimum_distance_of_touching_neighbors_dilated_r_10
// - maximum_distance_of_n10_nearest_neighbors
// - max_min_distance_ratio_of_touching_neighbors_dilated_r_10
// - average_distance_of_n8_nearest_neighbors
// - average_distance_of_touching_neighbors
// - maximum_distance_of_n8_nearest_neighbors
// - maximum_distance_of_n20_nearest_neighbors
// - maximum_distance_of_n7_nearest_neighbors
// - average_distance_of_n6_nearest_neighbors
// - average_distance_of_touching_neighbors_dilated_r_10
// - maximum_distance_of_n6_nearest_neighbors
// - maximum_distance_of_n5_nearest_neighbors
// - max_min_distance_ratio_of_touching_neighbors_dilated_r_5
// - average_distance_of_n4_nearest_neighbors
// - average_distance_of_n20_nearest_neighbors
// - average_distance_of_n3_nearest_neighbors
// - maximum_distance_of_n3_nearest_neighbors
// - touch_portion_above_0.200000_neighbor_count
// - average_distance_of_n2_nearest_neighbors
// - standard_deviation_touch_portion
// - maximum_distance_of_n2_nearest_neighbors
// - maximum_distance_of_n1_nearest_neighbors
// - label
// - proximal_neighbor_count_d160
// - proximal_neighbor_count_d80
// - proximal_neighbor_count_d40
// - average_distance_of_n5_nearest_neighbors
// - proximal_neighbor_count_d20
// - maximum_distance_of_touching_neighbors


auto
touching_neighbor_count_map_func(const Device::Pointer & device, const Array::Pointer & labels, Array::Pointer dst) -> Array::Pointer
{
  auto props = cle::tier7::labels_neighbors_statistics_func(device, labels, {}, {}, {}, true);
  return cle::tier4::parametric_map_func(device, labels, props, dst, std::string("touching_neighbor_count"));
}


namespace
{
template <typename OperationFunc>
auto
apply_touching_neighbors_operation(const Device::Pointer & device,
                                   const Array::Pointer &  map,
                                   const Array::Pointer &  labels,
                                   Array::Pointer &        dst,
                                   int                     radius,
                                   bool                    ignore_background,
                                   OperationFunc           operation) -> Array::Pointer
{
  tier0::create_like(map, dst, dType::FLOAT);
  if (radius < 1)
  {
    tier1::copy_func(device, map, dst);
    return dst;
  }
  auto touch_matrix = tier3::generate_touch_matrix_func(device, labels, nullptr);
  if (ignore_background)
  {
    tier1::set_column_func(device, touch_matrix, 0, 0);
  }
  for (int r = 1; r < radius; r++)
  {
    touch_matrix = tier3::generate_touch_matrix_func(device, touch_matrix, nullptr);
    if (ignore_background)
    {
      tier1::set_column_func(device, touch_matrix, 0, 0);
    }
  }
  auto values = tier3::read_map_values_func(device, map, labels, nullptr);
  auto new_values = operation(device, values, touch_matrix, nullptr);
  tier1::set_column_func(device, new_values, 0, 0);
  tier1::replace_intensities_func(device, labels, new_values, dst);
  return dst;
}
} // anonymous namespace


auto
mean_of_touching_neighbors_map_func(const Device::Pointer & device,
                                    const Array::Pointer &  map,
                                    const Array::Pointer &  labels,
                                    Array::Pointer          dst,
                                    int                     radius,
                                    bool                    ignore_background) -> Array::Pointer
{
  return apply_touching_neighbors_operation(device, map, labels, dst, radius, ignore_background, tier1::mean_of_touching_neighbors_func);
}


auto
minimum_of_touching_neighbors_map_func(const Device::Pointer & device,
                                       const Array::Pointer &  map,
                                       const Array::Pointer &  labels,
                                       Array::Pointer          dst,
                                       int                     radius,
                                       bool                    ignore_background) -> Array::Pointer
{
  return apply_touching_neighbors_operation(device, map, labels, dst, radius, ignore_background, tier1::minimum_of_touching_neighbors_func);
}


auto
maximum_of_touching_neighbors_map_func(const Device::Pointer & device,
                                       const Array::Pointer &  map,
                                       const Array::Pointer &  labels,
                                       Array::Pointer          dst,
                                       int                     radius,
                                       bool                    ignore_background) -> Array::Pointer
{
  return apply_touching_neighbors_operation(device, map, labels, dst, radius, ignore_background, tier1::maximum_of_touching_neighbors_func);
}


auto
standard_deviation_of_touching_neighbors_map_func(const Device::Pointer & device,
                                                  const Array::Pointer &  map,
                                                  const Array::Pointer &  labels,
                                                  Array::Pointer          dst,
                                                  int                     radius,
                                                  bool                    ignore_background) -> Array::Pointer
{
  return apply_touching_neighbors_operation(
    device, map, labels, dst, radius, ignore_background, tier1::standard_deviation_of_touching_neighbors_func);
}


auto
mode_of_touching_neighbors_map_func(const Device::Pointer & device,
                                    const Array::Pointer &  map,
                                    const Array::Pointer &  labels,
                                    Array::Pointer          dst,
                                    int                     radius,
                                    bool                    ignore_background) -> Array::Pointer
{
  return apply_touching_neighbors_operation(device, map, labels, dst, radius, ignore_background, tier1::mode_of_touching_neighbors_func);
}


auto
median_of_touching_neighbors_map_func(const Device::Pointer & device,
                                      const Array::Pointer &  map,
                                      const Array::Pointer &  labels,
                                      Array::Pointer          dst,
                                      int                     radius,
                                      bool                    ignore_background) -> Array::Pointer
{
  return apply_touching_neighbors_operation(device, map, labels, dst, radius, ignore_background, tier1::median_of_touching_neighbors_func);
}

} // namespace cle::tier4
