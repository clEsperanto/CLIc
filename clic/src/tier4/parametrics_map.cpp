
#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier7.hpp"

#include "statistics.hpp"
#include "utils.hpp"

#include <algorithm>
#include <functional>
#include <numeric>

namespace cle::tier4
{

namespace
{

// Replace every label by values[label]; the background (index 0) becomes 0. replace_values does not check bounds, so
// labels without a value (values shorter than nb_labels) are mapped to 0 instead of being read out of bounds.
auto
map_label_values(const Device::Pointer & device,
                 const Array::Pointer &  labels,
                 std::vector<float>      values,
                 size_t                  nb_labels,
                 Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(labels, dst, dType::FLOAT);
  values.resize(std::max(values.size(), nb_labels), 0);
  values.front() = 0;
  auto lookup = Array::create(values.size(), 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  lookup->writeFrom(values.data());
  return tier1::replace_values_func(device, labels, lookup, dst);
}

// Map of a property of the first statistics pass only (see statistics_accumulate in statistics.cpp): plane 3 is the
// pixel count, 7 the sum, 8 the minimum and 9 the maximum intensity. With divide_by_count, the plane is divided by the
// pixel count, as compute_statistics_per_labels does (plane 7: mean intensity).
auto
first_pass_map(const Device::Pointer & device,
               const Array::Pointer &  labels,
               const Array::Pointer &  intensity,
               Array::Pointer          dst,
               size_t                  plane,
               bool                    divide_by_count) -> Array::Pointer
{
  const auto         nb_labels = static_cast<size_t>(tier1::maximum_of_all_pixels_func(device, labels)) + 1;
  const auto         stats = _statistics_per_label(device, labels, intensity, static_cast<int>(nb_labels));
  std::vector<float> values(nb_labels);
  stats->readTo(values.data(), { nb_labels, 1, 1 }, { 0, plane, 0 });
  if (divide_by_count)
  {
    std::vector<float> count(nb_labels);
    stats->readTo(count.data(), { nb_labels, 1, 1 }, { 0, 3, 0 });
    std::transform(values.begin(), values.end(), count.begin(), values.begin(), std::divides<float>());
  }
  return map_label_values(device, labels, std::move(values), nb_labels, dst);
}

// Map of any property of compute_statistics_per_labels (labels 1 to max, the background is not measured).
auto
statistics_map(const Device::Pointer & device,
               const Array::Pointer &  labels,
               const Array::Pointer &  intensity,
               Array::Pointer          dst,
               const std::string &     property) -> Array::Pointer
{
  auto values = compute_statistics_per_labels(device, labels, intensity).at(property);
  values.insert(values.begin(), 0);
  const auto nb_labels = values.size();
  return map_label_values(device, labels, std::move(values), nb_labels, dst);
}

} // namespace

auto
parametric_map_func(const Device::Pointer & device,
                    const Array::Pointer &  labels,
                    const StatisticsMap &   properties,
                    Array::Pointer          dst,
                    const std::string &     target_property) -> Array::Pointer
{
  // force property name to lower case
  std::string lower_property_name;
  std::transform(target_property.begin(), target_property.end(), std::back_inserter(lower_property_name), ::tolower);

  // Check if property exists
  if (properties.find(lower_property_name) == properties.end())
  {
    throw std::runtime_error("Property '" + target_property + "' not found in statistics");
  }

  const auto nb_labels = static_cast<size_t>(tier1::maximum_of_all_pixels_func(device, labels)) + 1;
  auto       vector = properties.at(lower_property_name);

  // properties measured without the background start at label 1
  if (vector.size() != nb_labels)
  {
    vector.insert(vector.begin(), 0);
  }
  return map_label_values(device, labels, std::move(vector), nb_labels, dst);
}


// -- Label props --
// The intensity-free maps pass the labels as intensity image: the properties they map do not depend on it.

auto
pixel_count_map_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return first_pass_map(device, src, src, dst, 3, false);
}

auto
label_pixel_count_map_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return pixel_count_map_func(device, src, dst);
}

auto
extension_ratio_map_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return statistics_map(device, src, src, dst, "mean_max_distance_to_centroid_ratio");
}

auto
mean_extension_map_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return statistics_map(device, src, src, dst, "mean_distance_to_centroid");
}

auto
maximum_extension_map_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return statistics_map(device, src, src, dst, "max_distance_to_centroid");
}

// -- Intensity props --

auto
mean_intensity_map_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & labels, Array::Pointer dst)
  -> Array::Pointer
{
  return first_pass_map(device, labels, src, dst, 7, true);
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
  return first_pass_map(device, labels, src, dst, 8, false);
}

auto
maximum_intensity_map_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & labels, Array::Pointer dst)
  -> Array::Pointer
{
  return first_pass_map(device, labels, src, dst, 9, false);
}

auto
standard_deviation_intensity_map_func(const Device::Pointer & device,
                                      const Array::Pointer &  src,
                                      const Array::Pointer &  labels,
                                      Array::Pointer          dst) -> Array::Pointer
{
  return statistics_map(device, labels, src, dst, "standard_deviation_intensity");
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
