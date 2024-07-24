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
                              Array::Pointer          intensity,
                              Array::Pointer          label,
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

  // initialize variables, output, and constants
  size_t offset = 1;
  size_t number_of_dimensions = label->dimension();
  size_t nb_labels = static_cast<size_t>(tier2::maximum_of_all_pixels_func(device, label)) + offset;
  size_t height = label->height();
  size_t depth = label->depth();
  size_t num_measurements = nb_labels - offset;

  std::unordered_map<std::string, std::vector<float>> region_props;
  std::vector<float>                                  temp_results(num_measurements);


  auto cumulative_stats_per_label = Array::create(nb_labels, height, 16, 3, dType::FLOAT, mType::BUFFER, device);
  cumulative_stats_per_label->fill(0);
  float min_value = std::numeric_limits<float>::lowest();
  float max_value = std::numeric_limits<float>::max();
  for (int i = 8; i <= 15; ++i)
  {
    float value = (i % 2 == 0) ? max_value : min_value;
    tier1::set_plane_func(device, cumulative_stats_per_label, i, value);
  }
  const KernelInfo kernel = { "statistics_per_label", kernel::statistics_per_label };
  const RangeArray range = { 1, height, 1 };
  ParameterList    params = { { "src_label", label },
                              { "src_image", intensity },
                              { "dst", cumulative_stats_per_label },
                              { "sum_background", 0 },
                              { "z", 0 } };
  for (int z = 0; z < depth; z++)
  {
    auto it = std::find_if(params.begin(), params.end(), [](const auto & param) { return param.first == "z"; });
    it->second = z;
    execute(device, kernel, params, range);
  }
  // collect slice-by-slice measurements in single planes
  auto sum_per_label = tier1::sum_y_projection_func(device, cumulative_stats_per_label, nullptr);
  auto min_per_label = tier1::minimum_y_projection_func(device, cumulative_stats_per_label, nullptr);
  auto max_per_label = tier1::maximum_y_projection_func(device, cumulative_stats_per_label, nullptr);

  auto label_statistics_image = Array::create(nb_labels, 8, 1, 2, dType::FLOAT, mType::BUFFER, device);
  auto result_vector = Array::create(num_measurements, 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  auto sum_dim = Array::create(result_vector);
  auto avg_dim = Array::create(result_vector);

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

  // min and max bbox x, y, z
  min_per_label->read(temp_results.data(), { num_measurements, 1, 1 }, { offset, 10, 0 });
  region_props["bbox_min_x"] = temp_results;
  min_per_label->read(temp_results.data(), { num_measurements, 1, 1 }, { offset, 12, 0 });
  region_props["bbox_min_y"] = temp_results;
  min_per_label->read(temp_results.data(), { num_measurements, 1, 1 }, { offset, 14, 0 });
  region_props["bbox_min_z"] = temp_results;
  max_per_label->read(temp_results.data(), { num_measurements, 1, 1 }, { offset, 11, 0 });
  region_props["bbox_max_x"] = temp_results;
  max_per_label->read(temp_results.data(), { num_measurements, 1, 1 }, { offset, 13, 0 });
  region_props["bbox_max_y"] = temp_results;
  max_per_label->read(temp_results.data(), { num_measurements, 1, 1 }, { offset, 15, 0 });
  region_props["bbox_max_z"] = temp_results;
  // bbox width, height, depth
  std::vector<float> bbox_width(num_measurements);
  std::vector<float> bbox_height(num_measurements);
  std::vector<float> bbox_depth(num_measurements);
  for (int i = 0; i < bbox_width.size(); ++i)
  {
    bbox_width[i] = region_props["bbox_max_x"][i] - region_props["bbox_min_x"][i] + 1;
    bbox_height[i] = region_props["bbox_max_y"][i] - region_props["bbox_min_y"][i] + 1;
    bbox_depth[i] = region_props["bbox_max_z"][i] - region_props["bbox_min_z"][i] + 1;
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

  // Area, minimum, maximum, sum, and mean intensity
  sum_per_label->copy(sum_dim, { num_measurements, 1, 1 }, { offset, 3, 0 }, { 0, 0, 0 });
  sum_dim->read(temp_results.data());
  region_props["area"] = temp_results;
  min_per_label->read(temp_results.data(), { num_measurements, 1, 1 }, { offset, 8, 0 });
  region_props["min_intensity"] = temp_results;
  max_per_label->read(temp_results.data(), { num_measurements, 1, 1 }, { offset, 9, 0 });
  region_props["max_intensity"] = temp_results;
  sum_per_label->copy(result_vector, { num_measurements, 1, 1 }, { offset, 7, 0 }, { 0, 0, 0 });
  result_vector->read(temp_results.data());
  region_props["sum_intensity"] = temp_results;
  tier1::paste_func(device, sum_dim, label_statistics_image, offset, 7, 0);
  tier1::divide_images_func(device, result_vector, sum_dim, avg_dim);
  tier1::paste_func(device, avg_dim, label_statistics_image, offset, 6, 0);
  avg_dim->read(temp_results.data());
  region_props["mean_intensity"] = temp_results;

  // Sum intensity times x, y, z and mass center
  std::vector<std::string> dim_names = { "x", "y", "z" };
  sum_per_label->copy(result_vector, { num_measurements, 1, 1 }, { offset, 4 + 3, 0 }, { 0, 0, 0 });
  for (int dim = 0; dim < 3; ++dim)
  {
    sum_per_label->copy(sum_dim, { num_measurements, 1, 1 }, { offset, 4 + static_cast<size_t>(dim), 0 }, { 0, 0, 0 });
    sum_dim->read(temp_results.data());
    region_props["sum_intensity_times_" + dim_names[dim]] = temp_results;
    tier1::divide_images_func(device, sum_dim, result_vector, avg_dim);
    avg_dim->read(temp_results.data());
    region_props["mass_center_" + dim_names[dim]] = temp_results;
    tier1::paste_func(device, avg_dim, label_statistics_image, offset, 3 + dim, 0);
  }

  // Sum x, y, z and centroid
  sum_per_label->copy(result_vector, { num_measurements, 1, 1 }, { offset, 3, 0 }, { 0, 0, 0 });
  for (int dim = 0; dim < 3; ++dim)
  {
    sum_per_label->copy(sum_dim, { num_measurements, 1, 1 }, { offset, static_cast<size_t>(dim), 0 }, { 0, 0, 0 });
    sum_dim->read(temp_results.data());
    region_props["sum_" + dim_names[dim]] = temp_results;
    tier1::divide_images_func(device, sum_dim, result_vector, avg_dim);
    avg_dim->read(temp_results.data());
    region_props["centroid_" + dim_names[dim]] = temp_results;
    tier1::paste_func(device, avg_dim, label_statistics_image, offset, dim, 0);
  }

  // Second part: determine parameters which depend on other parameters
  auto label_statistics_stack = Array::create(nb_labels, height, 6, 3, dType::FLOAT, mType::BUFFER, device);
  label_statistics_stack->fill(0);
  const KernelInfo kernel_std = { "standard_deviation_per_label", kernel::standard_deviation_per_label };
  const RangeArray range_std = { 1, height, 1 };
  ParameterList    params_std = { { "src_statistics", label_statistics_image },
                                  { "src_label", label },
                                  { "src_image", intensity },
                                  { "dst", label_statistics_stack },
                                  { "sum_background", 0 },
                                  { "z", 0 } };
  for (int z = 0; z < depth; z++)
  {
    auto it = std::find_if(params_std.begin(), params_std.end(), [](const auto & param) { return param.first == "z"; });
    it->second = z;
    execute(device, kernel_std, params_std, range_std);
  }
  auto sum_statistics = tier1::sum_y_projection_func(device, label_statistics_stack, nullptr);
  auto max_statistics = tier1::maximum_y_projection_func(device, label_statistics_stack, nullptr);
  sum_per_label->copy(result_vector, { num_measurements, 1, 1 }, { offset, 3, 0 }, { 0, 0, 0 });

  // Sum and mean distance to centroid
  sum_statistics->copy(sum_dim, { num_measurements, 1, 1 }, { offset, 0, 0 }, { 0, 0, 0 });
  sum_dim->read(temp_results.data());
  region_props["sum_distance_to_centroid"] = temp_results;
  tier1::divide_images_func(device, sum_dim, result_vector, avg_dim);
  avg_dim->read(temp_results.data());
  region_props["mean_distance_to_centroid"] = temp_results;
  // Sum and mean distance to center of mass
  sum_statistics->copy(sum_dim, { num_measurements, 1, 1 }, { offset, 1, 0 }, { 0, 0, 0 });
  sum_dim->read(temp_results.data());
  region_props["sum_distance_to_mass_center"] = temp_results;
  tier1::divide_images_func(device, sum_dim, result_vector, avg_dim);
  avg_dim->read(temp_results.data());
  region_props["mean_distance_to_mass_center"] = temp_results;
  // Standard deviation intensity
  sum_statistics->copy(sum_dim, { num_measurements, 1, 1 }, { offset, 2, 0 }, { 0, 0, 0 });
  tier1::power_func(device, sum_dim, result_vector, 0.5f);
  result_vector->read(temp_results.data());
  region_props["standard_deviation_intensity"] = temp_results;
  // Max distance to centroid and center of mass
  max_statistics->read(temp_results.data(), { num_measurements, 1, 1 }, { offset, 4, 0 });
  region_props["max_distance_to_centroid"] = temp_results;
  max_statistics->read(temp_results.data(), { num_measurements, 1, 1 }, { offset, 5, 0 });
  region_props["max_distance_to_mass_center"] = temp_results;

  // Calculate distance ratios
  std::vector<float> mean_max_distance_to_centroid_ratio(num_measurements);
  std::vector<float> mean_max_distance_to_mass_center_ratio(num_measurements);
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
