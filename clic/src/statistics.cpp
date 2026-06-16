#include "statistics.hpp"

#include "cle_standard_deviation_per_label.h"
#include "cle_statistics_per_label.h"

#include "execution.hpp"
#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"
#include "tier6.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>
namespace cle
{


auto
_statistics_per_label(const Device::Pointer & device, const Array::Pointer & label, const Array::Pointer & intensity, int nb_labels)
  -> Array::Pointer
{
  constexpr float min_value = std::numeric_limits<float>::lowest();
  constexpr float max_value = std::numeric_limits<float>::max();
  const size_t    height = label->height();
  const size_t    depth = label->depth();

  auto cumulative_stats_per_label = Array::create(nb_labels, height, 16, 3, dType::FLOAT, mType::BUFFER, device);
  cumulative_stats_per_label->fill(0);
  for (int i = 8; i <= 15; ++i)
  {
    float value = (i % 2 == 0) ? max_value : min_value;
    tier1::set_plane_func(device, cumulative_stats_per_label, i, value);
  }
  const KernelInfo kernel = { "statistics_per_label", kernel::statistics_per_label };
  const RangeArray range = { 1, height, 1 };
  ParameterList    params = {
    { "src_label", label }, { "src_image", intensity }, { "dst", cumulative_stats_per_label }, { "sum_background", 0 }, { "z", 0 }
  };
  for (int z = 0; z < depth; z++)
  {
    auto it = std::find_if(params.begin(), params.end(), [](const auto & param) { return param.first == "z"; });
    it->second = z;
    execute(device, kernel, params, range);
  }

  return cumulative_stats_per_label;
}

auto
_std_per_label(const Device::Pointer & device,
               const Array::Pointer &  statistics,
               const Array::Pointer &  label,
               const Array::Pointer &  intensity,
               int                     nb_labels) -> Array::Pointer
{
  const auto height = label->height();
  const auto depth = label->depth();

  auto label_statistics_stack = Array::create(nb_labels, height, 6, 3, dType::FLOAT, mType::BUFFER, device);
  label_statistics_stack->fill(0);
  const KernelInfo kernel_std = { "standard_deviation_per_label", kernel::standard_deviation_per_label };
  const RangeArray range_std = { 1, height, 1 };
  ParameterList    params_std = { { "src_statistics", statistics },  { "src_label", label },  { "src_image", intensity },
                                  { "dst", label_statistics_stack }, { "sum_background", 0 }, { "z", 0 } };
  for (int z = 0; z < depth; z++)
  {
    auto it = std::find_if(params_std.begin(), params_std.end(), [](const auto & param) { return param.first == "z"; });
    it->second = z;
    execute(device, kernel_std, params_std, range_std);
  }

  return label_statistics_stack;
}

auto
compute_statistics_per_labels(const Device::Pointer & device, const Array::Pointer & label, const Array::Pointer & intensity)
  -> StatisticsMap
{
  // initialize variables, output, and constants
  const auto       offset = 1;
  const auto       nb_labels = static_cast<size_t>(tier2::maximum_of_all_pixels_func(device, label)) + offset;
  const auto       nb_measurements = nb_labels - offset;
  const RangeArray origin = { 0, 0, 0 };
  const RangeArray region = { nb_measurements, 1, 1 };

  // create output map and temp vector in GPU and CPU
  cle::StatisticsMap region_props;
  region_props.reserve(37); // 37 properties are calculated (36 actually, we are missing 'original labels')
  auto result_device_vector = Array::create(nb_measurements, 1, 1, 1, dType::FLOAT, mType::BUFFER, device);

  // compute statistics per label and collect slice-by-slice measurements in single planes
  auto cumulative_stats_per_label = _statistics_per_label(device, label, intensity, nb_labels);
  auto sum_per_label = tier1::sum_y_projection_func(device, cumulative_stats_per_label, nullptr);
  auto min_per_label = tier1::minimum_y_projection_func(device, cumulative_stats_per_label, nullptr);
  auto max_per_label = tier1::maximum_y_projection_func(device, cumulative_stats_per_label, nullptr);

  auto label_statistics_image = Array::create(nb_labels, 8, 1, 2, dType::FLOAT, mType::BUFFER, device);
  auto sum_over_dimensions = Array::create(result_device_vector);
  auto avg_over_dimensions = Array::create(result_device_vector);

  // 0 labels
  std::vector<float> labels_list(nb_measurements);
  std::iota(labels_list.begin(), labels_list.end(), static_cast<float>(offset));
  region_props["label"] = std::move(labels_list);

  // 1-6 bbox x, y, z, width, height, depth
  /*
    # 10. min_x: minimum x coordinate of the label (in the given column)
    # 11. max_x: maximum x coordinate of the label
    # 12. min_y: minimum y coordinate of the label
    # 13. max_y: maximum y coordinate of the label
    # 14. min_z: minimum z coordinate of the label
    # 15. max_z: maximum z coordinate of the label
  */
  // min and max bbox x, y, z
  std::vector<float> bbox_min_x(nb_measurements);
  min_per_label->readTo(bbox_min_x.data(), region, { offset, 10, 0 });
  region_props["bbox_min_x"] = std::move(bbox_min_x);
  std::vector<float> bbox_min_y(nb_measurements);
  min_per_label->readTo(bbox_min_y.data(), region, { offset, 12, 0 });
  region_props["bbox_min_y"] = std::move(bbox_min_y);
  std::vector<float> bbox_min_z(nb_measurements);
  min_per_label->readTo(bbox_min_z.data(), region, { offset, 14, 0 });
  region_props["bbox_min_z"] = std::move(bbox_min_z);
  std::vector<float> bbox_max_x(nb_measurements);
  max_per_label->readTo(bbox_max_x.data(), region, { offset, 11, 0 });
  region_props["bbox_max_x"] = std::move(bbox_max_x);
  std::vector<float> bbox_max_y(nb_measurements);
  max_per_label->readTo(bbox_max_y.data(), region, { offset, 13, 0 });
  region_props["bbox_max_y"] = std::move(bbox_max_y);
  std::vector<float> bbox_max_z(nb_measurements);
  max_per_label->readTo(bbox_max_z.data(), region, { offset, 15, 0 });
  region_props["bbox_max_z"] = std::move(bbox_max_z);
  // bbox width, height, depth
  std::vector<float> bbox_width(nb_measurements);
  std::vector<float> bbox_height(nb_measurements);
  std::vector<float> bbox_depth(nb_measurements);
  for (int i = 0; i < bbox_width.size(); ++i)
  {
    bbox_width[i] = region_props["bbox_max_x"][i] - region_props["bbox_min_x"][i] + 1;
    bbox_height[i] = region_props["bbox_max_y"][i] - region_props["bbox_min_y"][i] + 1;
    bbox_depth[i] = region_props["bbox_max_z"][i] - region_props["bbox_min_z"][i] + 1;
  }
  region_props["bbox_width"] = std::move(bbox_width);
  region_props["bbox_height"] = std::move(bbox_height);
  region_props["bbox_depth"] = std::move(bbox_depth);

  /*
    # 0. sum_x: all x-coordinates summed per label (and column)
    # 1. sum_y: all y-coordinates summed per label
    # 2. sum_z: all z-coordinates summed per label
    # 3. sum: number of pixels per label (sum of a binary image representing the label)
    # 4. sum_intensity_x: intensity (of the intensity image) times x-coordinate summed per label
    # 5. sum_intensity_y: intensity times y-coordinate summed per label
    # 6. sum_intensity_z: intensity times z-coordinate summed per label
    # 7. sum_intensity: sum intensity (a.k.a. total intensity) of the label
    # 8. min_intensity: minimum intensity of the label
    # 9. max_intensity: maximum intensity of the label
  */
  // Area, minimum, maximum, sum, and mean intensity
  std::vector<float> area(nb_measurements);
  sum_per_label->copyTo(sum_over_dimensions, region, { offset, 3, 0 }, origin);
  sum_over_dimensions->readTo(area.data());
  region_props["area"] = std::move(area);
  std::vector<float> min_intensity(nb_measurements);
  min_per_label->readTo(min_intensity.data(), region, { offset, 8, 0 });
  region_props["min_intensity"] = std::move(min_intensity);
  std::vector<float> max_intensity(nb_measurements);
  max_per_label->readTo(max_intensity.data(), region, { offset, 9, 0 });
  region_props["max_intensity"] = std::move(max_intensity);
  std::vector<float> sum_intensity(nb_measurements);
  sum_per_label->copyTo(result_device_vector, region, { offset, 7, 0 }, origin);
  result_device_vector->readTo(sum_intensity.data());
  region_props["sum_intensity"] = sum_intensity;
  std::vector<float> mean_intensity(std::move(nb_measurements));
  tier1::paste_func(device, sum_over_dimensions, label_statistics_image, offset, 7, 0);
  tier1::divide_images_func(device, result_device_vector, sum_over_dimensions, avg_over_dimensions);
  tier1::paste_func(device, avg_over_dimensions, label_statistics_image, offset, 6, 0);
  avg_over_dimensions->readTo(mean_intensity.data());
  region_props["mean_intensity"] = std::move(mean_intensity);

  // Sum intensity times x, y, z and mass center
  std::vector<std::string> dim_names = { "x", "y", "z" };
  sum_per_label->copyTo(result_device_vector, region, { offset, 4 + 3, 0 }, origin);
  for (size_t dim = 0; dim < 3; ++dim)
  {
    std::vector<float> sum_intensity_times_(nb_measurements);
    sum_per_label->copyTo(sum_over_dimensions, region, { offset, 4 + dim, 0 }, origin);
    sum_over_dimensions->readTo(sum_intensity_times_.data());
    region_props["sum_intensity_times_" + dim_names[dim]] = std::move(sum_intensity_times_);
    std::vector<float> mass_center_(nb_measurements);
    tier1::divide_images_func(device, sum_over_dimensions, result_device_vector, avg_over_dimensions);
    avg_over_dimensions->readTo(mass_center_.data());
    region_props["mass_center_" + dim_names[dim]] = std::move(mass_center_);
    tier1::paste_func(device, avg_over_dimensions, label_statistics_image, offset, 3 + dim, 0);
  }
  // Sum x, y, z and centroid
  sum_per_label->copyTo(result_device_vector, region, { offset, 3, 0 }, origin);
  for (size_t dim = 0; dim < 3; ++dim)
  {
    std::vector<float> sum_(nb_measurements);
    sum_per_label->copyTo(sum_over_dimensions, region, { offset, dim, 0 }, origin);
    sum_over_dimensions->readTo(sum_.data());
    region_props["sum_" + dim_names[dim]] = std::move(sum_);
    std::vector<float> centroid_(nb_measurements);
    tier1::divide_images_func(device, sum_over_dimensions, result_device_vector, avg_over_dimensions);
    avg_over_dimensions->readTo(centroid_.data());
    region_props["centroid_" + dim_names[dim]] = std::move(centroid_);
    tier1::paste_func(device, avg_over_dimensions, label_statistics_image, offset, dim, 0);
  }

  // Second part: determine parameters which depend on other parameters
  auto label_statistics_stack = _std_per_label(device, label_statistics_image, label, intensity, nb_labels);
  auto sum_statistics = tier1::sum_y_projection_func(device, label_statistics_stack, nullptr);
  auto max_statistics = tier1::maximum_y_projection_func(device, label_statistics_stack, nullptr);
  sum_per_label->copyTo(result_device_vector, region, { offset, 3, 0 }, origin);

  // Sum and mean distance to centroid
  std::vector<float> sum_distance_to_centroid(nb_measurements);
  sum_statistics->copyTo(sum_over_dimensions, region, { offset, 0, 0 }, origin);
  sum_over_dimensions->readTo(sum_distance_to_centroid.data());
  region_props["sum_distance_to_centroid"] = std::move(sum_distance_to_centroid);
  std::vector<float> mean_distance_to_centroid(nb_measurements);
  tier1::divide_images_func(device, sum_over_dimensions, result_device_vector, avg_over_dimensions);
  avg_over_dimensions->readTo(mean_distance_to_centroid.data());
  region_props["mean_distance_to_centroid"] = std::move(mean_distance_to_centroid);
  // Sum and mean distance to center of mass
  std::vector<float> sum_distance_to_mass_center(nb_measurements);
  sum_statistics->copyTo(sum_over_dimensions, region, { offset, 1, 0 }, origin);
  sum_over_dimensions->readTo(sum_distance_to_mass_center.data());
  region_props["sum_distance_to_mass_center"] = std::move(sum_distance_to_mass_center);
  std::vector<float> mean_distance_to_mass_center(nb_measurements);
  tier1::divide_images_func(device, sum_over_dimensions, result_device_vector, avg_over_dimensions);
  avg_over_dimensions->readTo(mean_distance_to_mass_center.data());
  region_props["mean_distance_to_mass_center"] = std::move(mean_distance_to_mass_center);
  // Standard deviation intensity
  std::vector<float> standard_deviation_intensity(nb_measurements);
  sum_statistics->copyTo(sum_over_dimensions, region, { offset, 2, 0 }, origin);
  tier1::power_func(device, sum_over_dimensions, result_device_vector, 0.5f);
  result_device_vector->readTo(standard_deviation_intensity.data());
  region_props["standard_deviation_intensity"] = std::move(standard_deviation_intensity);
  // Max distance to centroid and center of mass
  std::vector<float> max_distance_to_centroid(nb_measurements);
  max_statistics->readTo(max_distance_to_centroid.data(), region, { offset, 4, 0 });
  region_props["max_distance_to_centroid"] = std::move(max_distance_to_centroid);
  std::vector<float> max_distance_to_mass_center(nb_measurements);
  max_statistics->readTo(max_distance_to_mass_center.data(), region, { offset, 5, 0 });
  region_props["max_distance_to_mass_center"] = std::move(max_distance_to_mass_center);

  // Calculate distance ratios
  std::vector<float> mean_max_distance_to_centroid_ratio(nb_measurements);
  std::vector<float> mean_max_distance_to_mass_center_ratio(nb_measurements);
  for (int i = 0; i < nb_measurements; ++i)
  {
    mean_max_distance_to_centroid_ratio[i] = region_props["max_distance_to_centroid"][i] / region_props["mean_distance_to_centroid"][i];
    mean_max_distance_to_mass_center_ratio[i] =
      region_props["max_distance_to_mass_center"][i] / region_props["mean_distance_to_mass_center"][i];
  }
  region_props["mean_max_distance_to_centroid_ratio"] = std::move(mean_max_distance_to_centroid_ratio);
  region_props["mean_max_distance_to_mass_center_ratio"] = std::move(mean_max_distance_to_mass_center_ratio);

  return region_props;
}


auto
compute_neighbors_statistics_per_labels(const Device::Pointer &  device,
                                        const Array::Pointer &   label,
                                        const std::vector<int> & proximal_distances,
                                        const std::vector<int> & nearest_neighbor_ns,
                                        const std::vector<int> & dilation_radii) -> StatisticsMap
{
  const std::array<float, 6> touch_ratio_thresholds = { 0, 0.16, 0.2, 0.33, 0.5, 0.75 };

  const auto nb_labels = static_cast<size_t>(tier2::maximum_of_all_pixels_func(device, label)) + 1;
  const auto nb_proximal_distances = proximal_distances.size();
  const auto nb_nearest_neighbor_ns = nearest_neighbor_ns.size();
  const auto nb_dilation_radii = dilation_radii.size();
  const auto nb_touch_ratio_thresholds = touch_ratio_thresholds.size();

  const auto         nb_stats = nb_proximal_distances + 2 * nb_nearest_neighbor_ns + 6 * nb_dilation_radii + nb_touch_ratio_thresholds + 13;
  cle::StatisticsMap nei_stats;
  nei_stats.reserve(nb_stats); // 37 properties are calculated (36 actually, we are missing 'original labels')

  auto result_device_vector = Array::create(nb_labels, 1, 1, 1, dType::FLOAT, mType::BUFFER, device);

  auto centroids = tier4::centroids_of_labels_func(device, label, nullptr, false);
  auto distance_matrix = tier1::generate_distance_matrix_func(device, centroids, centroids, nullptr);
  auto touching_matrix = tier3::generate_touch_matrix_func(device, label, nullptr);
  tier1::set_column_func(device, touching_matrix, 0, 0);
  tier1::set_row_func(device, touching_matrix, 0, 0);

  // labels include background #0 during computation
  std::vector<float> labels_list(nb_labels);
  std::iota(labels_list.begin(), labels_list.end(), 0);
  nei_stats["label"] = std::move(labels_list);

  // touching neighbor count # 1
  tier2::count_touching_neighbors_func(device, touching_matrix, result_device_vector, true);
  auto & touching_neighbor_count = nei_stats["touching_neighbor_count"];
  touching_neighbor_count.resize(nb_labels);
  result_device_vector->readTo(touching_neighbor_count.data());

  // distances of touching neighbors # 2-4
  tier1::minimum_distance_touching_neighbors_func(device, distance_matrix, touching_matrix, result_device_vector);
  auto & minimum_distance_of_touching_neighbors = nei_stats["minimum_distance_of_touching_neighbors"];
  minimum_distance_of_touching_neighbors.resize(nb_labels);
  result_device_vector->readTo(minimum_distance_of_touching_neighbors.data());
  tier1::average_distance_touching_neighbors_func(device, distance_matrix, touching_matrix, result_device_vector);
  auto & average_distance_of_touching_neighbors = nei_stats["average_distance_of_touching_neighbors"];
  average_distance_of_touching_neighbors.resize(nb_labels);
  result_device_vector->readTo(average_distance_of_touching_neighbors.data());
  tier1::maximum_distance_touching_neighbors_func(device, distance_matrix, touching_matrix, result_device_vector);
  auto & maximum_distance_of_touching_neighbors = nei_stats["maximum_distance_of_touching_neighbors"];
  maximum_distance_of_touching_neighbors.resize(nb_labels);
  result_device_vector->readTo(maximum_distance_of_touching_neighbors.data());

  // max/min distance ratio of touching neighbors # 5
  auto & max_min_distance_ratio_of_touching_neighbors = nei_stats["max_min_distance_ratio_of_touching_neighbors"];
  max_min_distance_ratio_of_touching_neighbors.resize(nb_labels);
  std::transform(nei_stats["maximum_distance_of_touching_neighbors"].begin(),
                 nei_stats["maximum_distance_of_touching_neighbors"].end(),
                 nei_stats["minimum_distance_of_touching_neighbors"].begin(),
                 max_min_distance_ratio_of_touching_neighbors.begin(),
                 [](float max_dist, float min_dist) { return max_dist / min_dist; });

  // number of neighbors within given radii # 6-...
  for (auto d : proximal_distances)
  {
    auto proximal_touch_matrix = tier2::generate_proximal_neighbors_matrix_func(device, distance_matrix, nullptr, 0, d);
    tier2::count_touching_neighbors_func(device, proximal_touch_matrix, result_device_vector, true);
    auto & proximal_neighbor_count_d = nei_stats["proximal_neighbor_count_d" + std::to_string(d)];
    proximal_neighbor_count_d.resize(nb_labels);
    result_device_vector->readTo(proximal_neighbor_count_d.data());
  }

  auto self_far_distance_matrix = tier1::copy_func(device, distance_matrix, nullptr);
  auto max_distance = tier2::maximum_of_all_pixels_func(device, distance_matrix);
  tier1::set_where_x_equals_y_func(device, self_far_distance_matrix, max_distance + 1);

  // maximum and average distance of n nearest neighbors # ... and distance of most distant other neighbor
  for (auto n : nearest_neighbor_ns)
  {
    auto max_distance_of_n_shortest_distances =
      tier1::maximum_distance_n_nearest_neighbors_func(device, self_far_distance_matrix, nullptr, n);
    auto & maximum_distance_of_n_nearest_neighbors = nei_stats["maximum_distance_of_n" + std::to_string(n) + "_nearest_neighbors"];
    maximum_distance_of_n_nearest_neighbors.resize(nb_labels);
    max_distance_of_n_shortest_distances->readTo(maximum_distance_of_n_nearest_neighbors.data());

    auto average_distance_of_n_nearest_distances =
      tier1::mean_distance_n_nearest_neighbors_func(device, self_far_distance_matrix, nullptr, n);
    auto & average_distance_of_n_nearest_neighbors = nei_stats["average_distance_of_n" + std::to_string(n) + "_nearest_neighbors"];
    average_distance_of_n_nearest_neighbors.resize(nb_labels);
    average_distance_of_n_nearest_distances->readTo(average_distance_of_n_nearest_neighbors.data());
  }

  // distance to most distant other neighbor
  auto & distance_to_most_distant_other = nei_stats["distance_to_most_distant_other"];
  distance_to_most_distant_other.resize(nb_labels);
  tier1::mean_distance_n_farthest_neighbors_func(device, self_far_distance_matrix, result_device_vector, 1);
  result_device_vector->readTo(distance_to_most_distant_other.data());

  // touch count / portion
  auto touch_count_matrix = tier3::generate_touching_area_matrix_func(device, label, nullptr);
  auto touch_portion_matrix = tier4::generate_partial_touching_area_matrix_func(device, label, nullptr);
  tier1::set_column_func(device, touch_count_matrix, 0, 0);
  tier1::set_row_func(device, touch_count_matrix, 0, 0);
  tier1::set_column_func(device, touch_portion_matrix, 0, 0);
  tier1::set_row_func(device, touch_portion_matrix, 0, 0);

  // set all 0 pixels to a high value to detect the minimum touch count/portion
  auto max_touch_count = tier2::maximum_of_all_pixels_func(device, touch_count_matrix);
  auto touch_count_matrix_0_set_to_high = tier1::replace_intensity_func(device, touch_count_matrix, nullptr, 0, max_touch_count);
  for (auto t : touch_ratio_thresholds)
  {
    auto touch_portion_within_range_neighbors_matrix =
      tier2::generate_partial_touching_area_matrix_within_range_func(device, touch_portion_matrix, nullptr, t, 1.1f);
    tier2::count_touching_neighbors_func(device, touch_portion_within_range_neighbors_matrix, result_device_vector, true);
    auto & touch_portion_above_threshold_neighbor_count = nei_stats["touch_portion_above_" + std::to_string(t) + "_neighbor_count"];
    touch_portion_above_threshold_neighbor_count.resize(nb_labels);
    result_device_vector->readTo(touch_portion_above_threshold_neighbor_count.data());
  }

  auto temp = tier1::nan_to_num_func(device, touch_portion_matrix, nullptr);
  auto max_touch_portion = tier2::maximum_of_all_pixels_func(device, temp);
  auto touch_portion_matrix_0_set_to_high = tier1::replace_intensity_func(device, temp, nullptr, 0, max_touch_portion);

  auto & touch_count_sum = nei_stats["touch_count_sum"];
  touch_count_sum.resize(nb_labels);
  tier1::sum_y_projection_func(device, touch_count_matrix, result_device_vector);
  result_device_vector->readTo(touch_count_sum.data());
  auto & minimum_touch_count = nei_stats["minimum_touch_count"];
  minimum_touch_count.resize(nb_labels);
  tier1::minimum_y_projection_func(device, touch_count_matrix_0_set_to_high, result_device_vector);
  result_device_vector->readTo(minimum_touch_count.data());
  auto & maximum_touch_count = nei_stats["maximum_touch_count"];
  maximum_touch_count.resize(nb_labels);
  tier1::maximum_y_projection_func(device, touch_count_matrix, result_device_vector);
  result_device_vector->readTo(maximum_touch_count.data());
  auto & minimum_touch_portion = nei_stats["minimum_touch_portion"];
  minimum_touch_portion.resize(nb_labels);
  tier1::minimum_y_projection_func(device, touch_portion_matrix_0_set_to_high, result_device_vector);
  result_device_vector->readTo(minimum_touch_portion.data());
  auto & maximum_touch_portion = nei_stats["maximum_touch_portion"];
  maximum_touch_portion.resize(nb_labels);
  tier1::maximum_y_projection_func(device, touch_portion_matrix, result_device_vector);
  result_device_vector->readTo(maximum_touch_portion.data());

  auto & standard_deviation_touch_portion = nei_stats["standard_deviation_touch_portion"];
  standard_deviation_touch_portion.resize(nb_labels);
  tier5::standard_deviation_partial_touching_area_matrix_func(device, label, result_device_vector);
  result_device_vector->readTo(standard_deviation_touch_portion.data());

  // dilate label image and analye touching neighbors

  for (auto r : dilation_radii)
  {
    auto dilated_label_image = tier6::dilate_labels_func(device, label, nullptr, r);
    auto dilated_centroids = tier4::centroids_of_labels_func(device, dilated_label_image, nullptr, false);
    auto dilated_distance_matrix = tier1::generate_distance_matrix_func(device, dilated_centroids, dilated_centroids, nullptr);
    auto dilated_touch_matrix = tier3::generate_touch_matrix_func(device, dilated_label_image, nullptr);
    tier1::set_column_func(device, dilated_touch_matrix, 0, 0);
    tier1::set_row_func(device, dilated_touch_matrix, 0, 0);

    auto & touching_neighbor_count_dilated_r = nei_stats["touching_neighbor_count_dilated_r_" + std::to_string(r)];
    touching_neighbor_count_dilated_r.resize(nb_labels);
    tier2::count_touching_neighbors_func(device, dilated_touch_matrix, result_device_vector, true);
    result_device_vector->readTo(touching_neighbor_count_dilated_r.data());

    auto & minimum_distance_of_touching_neighbors_dilated_r =
      nei_stats["minimum_distance_of_touching_neighbors_dilated_r_" + std::to_string(r)];
    minimum_distance_of_touching_neighbors_dilated_r.resize(nb_labels);
    tier1::minimum_distance_touching_neighbors_func(device, dilated_distance_matrix, dilated_touch_matrix, result_device_vector);
    result_device_vector->readTo(minimum_distance_of_touching_neighbors_dilated_r.data());
    auto & average_distance_of_touching_neighbors_dilated_r =
      nei_stats["average_distance_of_touching_neighbors_dilated_r_" + std::to_string(r)];
    average_distance_of_touching_neighbors_dilated_r.resize(nb_labels);
    tier1::mean_distance_touching_neighbors_func(device, dilated_distance_matrix, dilated_touch_matrix, result_device_vector);
    result_device_vector->readTo(average_distance_of_touching_neighbors_dilated_r.data());
    auto & maximum_distance_of_touching_neighbors_dilated_r =
      nei_stats["maximum_distance_of_touching_neighbors_dilated_r_" + std::to_string(r)];
    maximum_distance_of_touching_neighbors_dilated_r.resize(nb_labels);
    tier1::maximum_distance_touching_neighbors_func(device, dilated_distance_matrix, dilated_touch_matrix, result_device_vector);
    result_device_vector->readTo(maximum_distance_of_touching_neighbors_dilated_r.data());


    auto & max_min_distance_ratio_of_touching_neighbors_dilated_r =
      nei_stats["max_min_distance_ratio_of_touching_neighbors_dilated_r_" + std::to_string(r)];
    max_min_distance_ratio_of_touching_neighbors_dilated_r.resize(nb_labels);
    std::transform(nei_stats["maximum_distance_of_touching_neighbors_dilated_r_" + std::to_string(r)].begin(),
                   nei_stats["maximum_distance_of_touching_neighbors_dilated_r_" + std::to_string(r)].end(),
                   nei_stats["minimum_distance_of_touching_neighbors_dilated_r_" + std::to_string(r)].begin(),
                   max_min_distance_ratio_of_touching_neighbors_dilated_r.begin(),
                   [](float max_dist, float min_dist) { return max_dist / min_dist; });

    auto dilated_touch_count_matrix = tier3::generate_touching_area_matrix_func(device, dilated_label_image, nullptr);
    auto dilated_touch_portion_matrix = tier4::generate_partial_touching_area_matrix_func(device, dilated_label_image, nullptr);
    tier1::set_column_func(device, dilated_touch_count_matrix, 0, 0);
    tier1::set_row_func(device, dilated_touch_count_matrix, 0, 0);
    tier1::set_column_func(device, dilated_touch_portion_matrix, 0, 0);
    tier1::set_row_func(device, dilated_touch_portion_matrix, 0, 0);

    auto max_dilated_touch_count = tier2::maximum_of_all_pixels_func(device, dilated_touch_count_matrix);
    auto dilated_touch_count_matrix_0_set_to_high =
      tier1::replace_intensity_func(device, dilated_touch_count_matrix, nullptr, 0, max_dilated_touch_count);
    auto temp = tier1::nan_to_num_func(device, dilated_touch_portion_matrix, nullptr);
    auto max_dilated_touch_portion = tier2::maximum_of_all_pixels_func(device, temp);
    auto dilated_touch_portion_matrix_0_set_to_high = tier1::replace_intensity_func(device, temp, nullptr, 0, max_dilated_touch_portion);

    auto & touch_count_sum_dilated_r = nei_stats["touch_count_sum_dilated_r_" + std::to_string(r)];
    touch_count_sum_dilated_r.resize(nb_labels);
    tier1::sum_y_projection_func(device, dilated_touch_count_matrix, result_device_vector);
    result_device_vector->readTo(touch_count_sum_dilated_r.data());
    auto & minimum_touch_count_dilated_r = nei_stats["minimum_touch_count_dilated_r_" + std::to_string(r)];
    minimum_touch_count_dilated_r.resize(nb_labels);
    tier1::minimum_y_projection_func(device, dilated_touch_count_matrix_0_set_to_high, result_device_vector);
    result_device_vector->readTo(minimum_touch_count_dilated_r.data());
    auto & maximum_touch_count_dilated_r = nei_stats["maximum_touch_count_dilated_r_" + std::to_string(r)];
    maximum_touch_count_dilated_r.resize(nb_labels);
    tier1::maximum_y_projection_func(device, dilated_touch_count_matrix, result_device_vector);
    result_device_vector->readTo(maximum_touch_count_dilated_r.data());
    auto & minimum_touch_portion_dilated_r = nei_stats["minimum_touch_portion_dilated_r_" + std::to_string(r)];
    minimum_touch_portion_dilated_r.resize(nb_labels);
    tier1::minimum_y_projection_func(device, dilated_touch_portion_matrix_0_set_to_high, result_device_vector);
    result_device_vector->readTo(minimum_touch_portion_dilated_r.data());
    auto & maximum_touch_portion_dilated_r = nei_stats["maximum_touch_portion_dilated_r_" + std::to_string(r)];
    maximum_touch_portion_dilated_r.resize(nb_labels);
    tier1::maximum_y_projection_func(device, dilated_touch_portion_matrix, result_device_vector);
    result_device_vector->readTo(maximum_touch_portion_dilated_r.data());
  }

  return nei_stats;
}

} // namespace cle
