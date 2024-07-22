#include "statistics.hpp"
#include <numeric>

#include "cle_standard_deviation_per_label.h"
#include "cle_statistics_per_label.h"
#include "execution.hpp"
#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

namespace cle
{


auto
statistics_of_labelled_pixels(const Device::Pointer & device,
                              Array::Pointer &        intensity,
                              Array::Pointer &        label,
                              Array::Pointer          result) -> std::unordered_map<std::string, std::vector<float>>
{
  // check input arguments
  if (intensity == nullptr || label == nullptr)
  {
    throw std::runtime_error("Error: either intensity or label must be set.");
  }
  if (label == nullptr)
  {
    std::cerr << "Warning: label is not set. Pixels will be treated as one object." << std::endl;
    tier0::create_like(intensity, label, dType::LABEL);
    label->fill(1);
  }
  if (intensity == nullptr)
  {
    std::cerr << "Warning: intensity is not set. Pixels will be treated as one object." << std::endl;
    tier0::create_like(label, intensity, dType::FLOAT);
    label->copy(label);
    // todo : I think one of these two should be intensity
  }


  int offset = 1;
  int number_of_dimensions = label->dimension();
  int nb_labels = static_cast<int>(tier2::maximum_of_all_pixels_func(device, label)) + offset;
  int height = label->height();
  int depth = 1;
  if (number_of_dimensions == 3)
  {
    depth = label->depth();
  }


  auto cumulative_stats_per_label = Array::create(nb_labels, height, 16, 3, dType::FLOAT, mType::BUFFER, device);
  cumulative_stats_per_label->fill(0);

  float min_value = -std::numeric_limits<float>::max();
  float max_value = std::numeric_limits<float>::max();

  std::cout << "min_value: " << min_value << std::endl;
  std::cout << "max_value: " << max_value << std::endl;

  tier1::set_plane_func(device, cumulative_stats_per_label, 8, max_value);
  tier1::set_plane_func(device, cumulative_stats_per_label, 9, min_value);
  tier1::set_plane_func(device, cumulative_stats_per_label, 10, max_value);
  tier1::set_plane_func(device, cumulative_stats_per_label, 11, min_value);
  tier1::set_plane_func(device, cumulative_stats_per_label, 12, max_value);
  tier1::set_plane_func(device, cumulative_stats_per_label, 13, min_value);
  tier1::set_plane_func(device, cumulative_stats_per_label, 14, max_value);
  tier1::set_plane_func(device, cumulative_stats_per_label, 15, min_value);

  const KernelInfo kernel = { "statistics_per_label", kernel::statistics_per_label };
  const RangeArray range = { 1, static_cast<size_t>(height), 1 };
  ParameterList    params = { { "src_label", label },
                              { "src_image", intensity },
                              { "dst", cumulative_stats_per_label },
                              { "sum_background", 0 },
                              { "z", 0 } };
  for (int z = 0; z < depth; z++)
  {
    params.back().second = z;
    execute(device, kernel, params, range);
  }

  // collect slice-by-slice measurements in single planes
  int num_measurements = nb_labels - offset;

  auto sum_per_label = tier1::sum_y_projection_func(device, cumulative_stats_per_label, nullptr);
  auto min_per_label = tier1::minimum_y_projection_func(device, cumulative_stats_per_label, nullptr);
  auto max_per_label = tier1::maximum_y_projection_func(device, cumulative_stats_per_label, nullptr);

  auto label_statistics_image = Array::create(nb_labels, 8, 1, 2, dType::FLOAT, mType::BUFFER, device);
  auto result_vector = Array::create(num_measurements, 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  auto sum_dim = Array::create(result_vector);
  auto avg_dim = Array::create(result_vector);

  std::unordered_map<std::string, std::vector<float>> region_props;

  // 0 labels
  std::vector<float> labels_list(num_measurements);
  std::iota(labels_list.begin(), labels_list.end(), offset);
  region_props["label"] = labels_list;

  // 1-6 bbox x, y, z, width, height, depth
  /*
    # 10. min_x: minimum x coordinate of the label (in the given column)
    # 11. max_x
    # 12. min_y
    # 13. max_y
    # 14. min_z
    # 15. max_z
  */

  std::vector<float> bbox_min_x(num_measurements);
  std::vector<float> bbox_max_x(num_measurements);
  std::vector<float> bbox_min_y(num_measurements);
  std::vector<float> bbox_max_y(num_measurements);
  std::vector<float> bbox_min_z(num_measurements);
  std::vector<float> bbox_max_z(num_measurements);

  tier1::crop_func(device, min_per_label, result_vector, offset, 10, 0, num_measurements, 1, 1);
  result_vector->read(bbox_min_x.data());
  tier1::crop_func(device, max_per_label, result_vector, offset, 11, 0, num_measurements, 1, 1);
  result_vector->read(bbox_max_x.data());
  tier1::crop_func(device, min_per_label, result_vector, offset, 12, 0, num_measurements, 1, 1);
  result_vector->read(bbox_min_y.data());
  tier1::crop_func(device, max_per_label, result_vector, offset, 13, 0, num_measurements, 1, 1);
  result_vector->read(bbox_max_y.data());
  tier1::crop_func(device, min_per_label, result_vector, offset, 14, 0, num_measurements, 1, 1);
  result_vector->read(bbox_min_z.data());
  tier1::crop_func(device, max_per_label, result_vector, offset, 15, 0, num_measurements, 1, 1);
  result_vector->read(bbox_max_z.data());

  region_props["bbox_min_x"] = bbox_min_x;
  region_props["bbox_max_x"] = bbox_max_x;
  region_props["bbox_min_y"] = bbox_min_y;
  region_props["bbox_max_y"] = bbox_max_y;
  region_props["bbox_min_z"] = bbox_min_z;
  region_props["bbox_max_z"] = bbox_max_z;

  // Calculate bbox width, height, depth
  std::vector<float> bbox_width(num_measurements);
  std::vector<float> bbox_height(num_measurements);
  std::vector<float> bbox_depth(num_measurements);

  for (size_t i = 0; i < bbox_width.size(); ++i)
  {
    bbox_width[i] = bbox_max_x[i] - bbox_min_x[i] + 1;
    bbox_height[i] = bbox_max_y[i] - bbox_min_y[i] + 1;
    bbox_depth[i] = bbox_max_z[i] - bbox_min_z[i] + 1;
  }

  region_props["bbox_width"] = bbox_width;
  region_props["bbox_height"] = bbox_height;
  region_props["bbox_depth"] = bbox_depth;

  /*
    # 0. sum_x: all x-coordinates summed per label (and column)
    # 1. sum_y:
    # 2. sum_z:
    # 3. sum: number of pixels per label (sum of a binary image representing the label)
    # 4. sum_intensity_x: intensity (of the intensity image) times x-coordinate summed per label
    # 5. sum_intensity_y
    # 6. sum_intensity_z
    # 7. sum_intensity: sum intensity (a.k.a. total intensity) of the label
    # 8. min_intensity
    # 9. max_intensity

  */

  // Minimum and maximum intensity
  std::vector<float> min_intensity(num_measurements);
  tier1::crop_func(device, min_per_label, result_vector, offset, 8, 0, num_measurements, 1, 1);
  result_vector->read(min_intensity.data());
  region_props["min_intensity"] = min_intensity;

  std::vector<float> max_intensity(num_measurements);
  tier1::crop_func(device, max_per_label, result_vector, offset, 9, 0, num_measurements, 1, 1);
  result_vector->read(max_intensity.data());
  region_props["max_intensity"] = max_intensity;

  // Sum intensity, area, and mean intensity
  std::vector<float> sum_intensity(num_measurements);
  tier1::crop_func(device, sum_per_label, result_vector, offset, 7, 0, num_measurements, 1, 1);
  result_vector->read(sum_intensity.data());
  region_props["sum_intensity"] = sum_intensity;

  std::vector<float> area(num_measurements);
  tier1::crop_func(device, sum_per_label, sum_dim, offset, 3, 0, num_measurements, 1, 1);
  sum_dim->read(area.data());
  region_props["area"] = area;

  std::vector<float> mean_intensity(num_measurements);
  tier1::paste_func(device, sum_dim, label_statistics_image, offset, 7, 0);
  tier1::divide_images_func(device, result_vector, sum_dim, avg_dim);
  avg_dim->read(mean_intensity.data());
  region_props["mean_intensity"] = mean_intensity;

  tier1::paste_func(device, avg_dim, label_statistics_image, offset, 6, 0);

  // Sum intensity times x, y, z and mass center

  std::vector<std::string> dim_names = { "x", "y", "z" };
  tier1::crop_func(device, sum_per_label, result_vector, offset, 4 + 3, 0, num_measurements, 1, 1);
  for (int dim = 0; dim < 3; ++dim)
  {
    tier1::crop_func(device, sum_per_label, sum_dim, offset, 4 + dim, 0, num_measurements, 1, 1);

    std::vector<float> sum_intensity_times(num_measurements);
    sum_dim->read(sum_intensity_times.data());
    region_props["sum_intensity_times_" + dim_names[dim]] = sum_intensity_times;

    tier1::divide_images_func(device, sum_dim, result_vector, avg_dim);

    std::vector<float> mass_center(num_measurements);
    avg_dim->read(mass_center.data());
    region_props["mass_center_" + dim_names[dim]] = mass_center;
    tier1::paste_func(device, avg_dim, label_statistics_image, offset, 3 + dim, 0);
  }

  // Sum x, y, z and centroid
  tier1::crop_func(device, sum_per_label, result_vector, offset, 3, 0, num_measurements, 1, 1);
  for (int dim = 0; dim < 3; ++dim)
  {
    tier1::crop_func(device, sum_per_label, sum_dim, offset, dim, 0, num_measurements, 1, 1);

    std::vector<float> sum(num_measurements);
    sum_dim->read(sum.data());
    region_props["sum_" + dim_names[dim]] = sum;
    tier1::divide_images_func(device, sum_dim, result_vector, avg_dim);

    std::vector<float> centroid(num_measurements);
    avg_dim->read(centroid.data());
    region_props["centroid_" + dim_names[dim]] = centroid;
    tier1::paste_func(device, avg_dim, label_statistics_image, offset, 3 + dim, 0);
  }

  // Second part: determine parameters which depend on other parameters
  std::cout << "nb_labels: " << nb_labels << std::endl;
  auto label_statistics_stack = Array::create(nb_labels, height, 6, 3, dType::FLOAT, mType::BUFFER, device);
  label_statistics_stack->fill(0);

  cle::print<float>(label_statistics_image, "label_statistics_image");

  const KernelInfo    kernel_std = { "standard_deviation_per_label", kernel::standard_deviation_per_label };
  const RangeArray    range_std = { 1, static_cast<size_t>(height), 1 };
  const ParameterList params_std = { { "dst", label_statistics_stack },
                                     { "src_statistics", label_statistics_image },
                                     { "src_label", label },
                                     { "src_image", intensity },
                                     { "sum_background", 0 },
                                     { "z", 0 } };

  for (int z = 0; z < depth; z++)
  {
    params.back().second = z;
    execute(device, kernel_std, params_std, range_std);
  }

  cle::print<float>(label_statistics_stack, "label_statistics_stack");

  auto sum_statistics = tier1::sum_y_projection_func(device, label_statistics_stack, nullptr);
  auto max_statistics = tier1::maximum_y_projection_func(device, label_statistics_stack, nullptr);

  // Area
  tier1::crop_func(device, sum_per_label, result_vector, offset, 3, 0, num_measurements, 1, 1);

  // Distance to centroid
  std::vector<float> sum_distance_to_centroid(num_measurements);
  tier1::crop_func(device, sum_statistics, sum_dim, offset, 0, 0, num_measurements, 1, 1);
  sum_dim->read(sum_distance_to_centroid.data());
  region_props["sum_distance_to_centroid"] = sum_distance_to_centroid;

  std::vector<float> mean_distance_to_centroid(num_measurements);
  tier1::divide_images_func(device, sum_dim, result_vector, avg_dim);
  avg_dim->read(mean_distance_to_centroid.data());
  region_props["mean_distance_to_centroid"] = mean_distance_to_centroid;

  // Distance to center of mass
  std::vector<float> sum_distance_to_mass_center(num_measurements);
  tier1::crop_func(device, sum_statistics, sum_dim, offset, 1, 0, num_measurements, 1, 1);
  sum_dim->read(sum_distance_to_mass_center.data());
  region_props["sum_distance_to_mass_center"] = sum_distance_to_mass_center;

  std::vector<float> mean_distance_to_mass_center(num_measurements);
  tier1::divide_images_func(device, sum_dim, result_vector, avg_dim);
  avg_dim->read(mean_distance_to_mass_center.data());
  region_props["mean_distance_to_mass_center"] = mean_distance_to_mass_center;

  // Standard deviation intensity
  std::vector<float> standard_deviation_intensity(num_measurements);
  tier1::crop_func(device, sum_statistics, sum_dim, offset, 2, 0, num_measurements, 1, 1);
  tier1::power_func(device, sum_dim, result_vector, 0.5f);
  result_vector->read(standard_deviation_intensity.data());
  region_props["standard_deviation_intensity"] = standard_deviation_intensity;

  std::vector<float> max_distance_to_centroid(num_measurements);
  tier1::crop_func(device, max_statistics, result_vector, offset, 4, 0, num_measurements, 1, 1);
  result_vector->read(max_distance_to_centroid.data());
  region_props["max_distance_to_centroid"] = max_distance_to_centroid;

  std::vector<float> max_distance_to_mass_center(num_measurements);
  tier1::crop_func(device, max_statistics, result_vector, offset, 5, 0, num_measurements, 1, 1);
  result_vector->read(max_distance_to_mass_center.data());
  region_props["max_distance_to_mass_center"] = max_distance_to_mass_center;

  // Calculate ratios
  std::vector<float> mean_max_distance_to_centroid_ratio(nb_labels - offset);
  std::vector<float> mean_max_distance_to_mass_center_ratio(nb_labels - offset);

  for (size_t i = 0; i < mean_max_distance_to_centroid_ratio.size(); ++i)
  {
    mean_max_distance_to_centroid_ratio[i] =
      region_props["max_distance_to_centroid"][i] / region_props["mean_distance_to_centroid"][i];
    mean_max_distance_to_mass_center_ratio[i] =
      region_props["max_distance_to_mass_center"][i] / region_props["mean_distance_to_mass_center"][i];
  }

  region_props["mean_max_distance_to_centroid_ratio"] = mean_max_distance_to_centroid_ratio;
  region_props["mean_max_distance_to_mass_center_ratio"] = mean_max_distance_to_mass_center_ratio;

  return region_props;
}


} // namespace cle
