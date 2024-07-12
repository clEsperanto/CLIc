#ifndef __INCLUDE_STATISTICS_HPP
#define __INCLUDE_STATISTICS_HPP


#include "array.hpp"
#include "execution.hpp"

#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

namespace cle
{


auto
compute_statistics(const Device::Pointer & device, Array::Pointer & intensity, Array::Pointer & label) -> void
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
  }


  int offset = 1;
  int number_of_dimensions = label->dimension();
  int nb_labels = static_cast<int>(tier2::maximum_of_all_pixels_func(label)) + offset;
  int height = label->height();
  int depth = 1;
  if (number_of_dimensions == 3)
  {
    depth = label->depth();
  }


  auto cumulative_stats_per_label = Array::Create(nb_labels, height, 16, 3, dType::FLOAT, mType::BUFFER, device);
  cumulative_stats_per_label->fill(0);

  float min_value = 0;
  float max_value = 0;

  tier1::set_plane(device, cumulative_stats_per_label, 8, min_value);
  tier1::set_plane(device, cumulative_stats_per_label, 9, min_value);
  tier1::set_plane(device, cumulative_stats_per_label, 10, min_value);
  tier1::set_plane(device, cumulative_stats_per_label, 11, min_value);
  tier1::set_plane(device, cumulative_stats_per_label, 12, min_value);
  tier1::set_plane(device, cumulative_stats_per_label, 13, min_value);
  tier1::set_plane(device, cumulative_stats_per_label, 14, min_value);
  tier1::set_plane(device, cumulative_stats_per_label, 15, min_value);

  const KernelInfo    kernel = { "binary_or", kernel::binary_or };
  const RangeArray    range = { 1, height, 1 };
  const ParameterList params = {
    { "src0", label }, { "src1", intensity }, { "dst", cumulative_stats_per_label }, { "sum", 0 }, { "depth", 0 }
  };
  for (int z = 0; z < depth; z++)
  {
    params.back()["depth"] = z;
    execute(device, kernel, params, range);
  }

  // collect slice-by-slice measurements in single planes
  auto sum_per_label = tier1::sum_y_projection(device, cumulative_stats_per_label_image, nullptr);
  auto min_per_label = tier1::minimum_y_projection(device, cumulative_stats_per_label_image, nullptr);
  auto max_per_label = tier1::maximum_y_projection(device, cumulative_stats_per_label_image, nullptr);

  auto label_statistics_image = Array::Create(nb_labels, 8, 1, 2, dType::FLOAT, mType::BUFFER, device);
  auto result_vector = Array::Create(nb_labels - offset, 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  auto sum_dim = Array::Create(result_vector);
  auto avg_dim = Array::Create(result_vector);

  std::unordered_map<std::string, std::vector<float>> region_props;

  // 0 labels
  std::vector<float> labels_list(nb_labels - offset);
  std::iota(labels_list.begin(), labels_list.end(), offset);
  region_props["label"] = labels_list;

  // 1-6 bbox x, y, z, width, height, depth
  tier1::crop(device, min_per_label, result_vector, offset, 10, 0);
  std::vector<float> bbox_min_x = result_vector->as_vector<float>();
  region_props["bbox_min_x"] = bbox_min_x;

  tier1::crop(device, min_per_label, result_vector, offset, 12, 0);
  std::vector<float> bbox_min_y = result_vector->as_vector<float>();
  region_props["bbox_min_y"] = bbox_min_y;

  tier1::crop(device, min_per_label, result_vector, offset, 14, 0);
  std::vector<float> bbox_min_z = result_vector->as_vector<float>();
  region_props["bbox_min_z"] = bbox_min_z;

  tier1::crop(device, max_per_label, result_vector, offset, 11, 0);
  std::vector<float> bbox_max_x = result_vector->as_vector<float>();
  region_props["bbox_max_x"] = bbox_max_x;

  tier1::crop(device, max_per_label, result_vector, offset, 13, 0);
  std::vector<float> bbox_max_y = result_vector->as_vector<float>();
  region_props["bbox_max_y"] = bbox_max_y;

  tier1::crop(device, max_per_label, result_vector, offset, 15, 0);
  std::vector<float> bbox_max_z = result_vector->as_vector<float>();
  region_props["bbox_max_z"] = bbox_max_z;

  // Calculate bbox width, height, depth
  std::vector<float> bbox_width(nb_labels - offset);
  std::vector<float> bbox_height(nb_labels - offset);
  std::vector<float> bbox_depth(nb_labels - offset);

  for (size_t i = 0; i < bbox_width.size(); ++i) {
    bbox_width[i] = bbox_max_x[i] - bbox_min_x[i] + 1;
    bbox_height[i] = bbox_max_y[i] - bbox_min_y[i] + 1;
    bbox_depth[i] = bbox_max_z[i] - bbox_min_z[i] + 1;
  }

  region_props["bbox_width"] = bbox_width;
  region_props["bbox_height"] = bbox_height;
  region_props["bbox_depth"] = bbox_depth;

  // Minimum and maximum intensity
  tier1::crop(device, min_per_label, result_vector, offset, 8, 0);
  region_props["min_intensity"] = result_vector->as_vector<float>();

  tier1::crop(device, max_per_label, result_vector, offset, 9, 0);
  region_props["max_intensity"] = result_vector->as_vector<float>();

  // Sum intensity, area, and mean intensity
  tier1::crop(device, sum_per_label, result_vector, offset, 7, 0);
  region_props["sum_intensity"] = result_vector->as_vector<float>();

  tier1::crop(device, sum_per_label, sum_dim, offset, 3, 0);
  region_props["area"] = sum_dim->as_vector<float>();
  tier1::paste(device, sum_dim, label_statistics_image, offset, 7, 0);

  tier1::divide_images(device, result_vector, sum_dim, avg_dim);
  region_props["mean_intensity"] = avg_dim->as_vector<float>();
  tier1::paste(device, avg_dim, label_statistics_image, offset, 6, 0);

  // Sum intensity times x, y, z and mass center
  std::vector<std::string> dim_names = {"x", "y", "z"};
  tier1::crop(device, sum_per_label, result_vector, offset, 4 + 3, 0);
  for (int dim = 0; dim < 3; ++dim) {
    tier1::crop(device, sum_per_label, sum_dim, offset, 4 + dim, 0);
    region_props["sum_intensity_times_" + dim_names[dim]] = sum_dim->as_vector<float>();
    tier1::divide_images(device, sum_dim, result_vector, avg_dim);
    region_props["mass_center_" + dim_names[dim]] = avg_dim->as_vector<float>();
    tier1::paste(device, avg_dim, label_statistics_image, offset, 3 + dim, 0);
  }

  // Sum x, y, z and centroid
  tier1::crop(device, sum_per_label, result_vector, offset, 3, 0);
  for (int dim = 0; dim < 3; ++dim) {
    tier1::crop(device, sum_per_label, sum_dim, offset, dim, 0);
    region_props["sum_" + dim_names[dim]] = sum_dim->as_vector<float>();
    tier1::divide_images(device, sum_dim, result_vector, avg_dim);
    region_props["centroid_" + dim_names[dim]] = avg_dim->as_vector<float>();
    tier1::paste(device, avg_dim, label_statistics_image, offset, dim, 0);
  }

  // Second part: determine parameters which depend on other parameters
  auto label_statistics_stack = Array::Create(nb_labels, height, 6, 3, dType::FLOAT, mType::BUFFER, device);
  label_statistics_stack->fill(0);

  const KernelInfo kernel_std = { "standard_deviation_per_label", kernel::standard_deviation_per_label };
  const RangeArray range_std = { 1, height, 1 };
  const ParameterList params_std = {
    { "dst", label_statistics_stack },
    { "src_statistics", label_statistics_image },
    { "src_label", label },
    { "src_image", intensity },
    { "sum_background", 0 },
    { "z", 0 }
  };

  for (int z = 0; z < depth; z++) {
    params_std.back()["z"] = z;
    execute(device, kernel_std, params_std, range_std);
  }

  auto sum_statistics = tier1::sum_y_projection(device, label_statistics_stack, nullptr);
  auto max_statistics = tier1::maximum_y_projection(device, label_statistics_stack, nullptr);

  // Distance to centroid
  tier1::crop(device, sum_statistics, sum_dim, offset, 0, 0);
  region_props["sum_distance_to_centroid"] = sum_dim->as_vector<float>();
  tier1::divide_images(device, sum_dim, result_vector, avg_dim);
  region_props["mean_distance_to_centroid"] = avg_dim->as_vector<float>();

  // Distance to center of mass
  tier1::crop(device, sum_statistics, sum_dim, offset, 1, 0);
  region_props["sum_distance_to_mass_center"] = sum_dim->as_vector<float>();
  tier1::divide_images(device, sum_dim, result_vector, avg_dim);
  region_props["mean_distance_to_mass_center"] = avg_dim->as_vector<float>();

  // Standard deviation intensity
  tier1::crop(device, sum_statistics, sum_dim, offset, 2, 0);
  tier1::power(device, sum_dim, result_vector, 0.5f);
  region_props["standard_deviation_intensity"] = result_vector->as_vector<float>();

  tier1::crop(device, max_statistics, result_vector, offset, 4, 0);
  region_props["max_distance_to_centroid"] = result_vector->as_vector<float>();
  tier1::crop(device, max_statistics, result_vector, offset, 5, 0);
  region_props["max_distance_to_mass_center"] = result_vector->as_vector<float>();

  // Calculate ratios
  std::vector<float> mean_max_distance_to_centroid_ratio(nb_labels - offset);
  std::vector<float> mean_max_distance_to_mass_center_ratio(nb_labels - offset);

  for (size_t i = 0; i < mean_max_distance_to_centroid_ratio.size(); ++i) {
    mean_max_distance_to_centroid_ratio[i] = region_props["max_distance_to_centroid"][i] / region_props["mean_distance_to_centroid"][i];
    mean_max_distance_to_mass_center_ratio[i] = region_props["max_distance_to_mass_center"][i] / region_props["mean_distance_to_mass_center"][i];
  }

  region_props["mean_max_distance_to_centroid_ratio"] = mean_max_distance_to_centroid_ratio;
  region_props["mean_max_distance_to_mass_center_ratio"] = mean_max_distance_to_mass_center_ratio;

  return region_props;
}


} // namespace cle

#endif // __INCLUDE_STATISTICS_HPP
