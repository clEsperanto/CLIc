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

  // // 1-6 bbox x, y, z, width, height, depth
  // tier1::crop(device, min_per_label, result_vector, offset, 10, 0);
  // bbox_min_x = pull(result_vector)[0];
  // region_props['bbox_min_x'] = bbox_min_x;

  // tier1::crop(device, min_per_label, result_vector, offset, 12, 0);
  // bbox_min_y = pull(result_vector)[0];
  // region_props['bbox_min_y'] = bbox_min_y;

  // tier1::crop(device, min_per_label, result_vector, offset, 14, 0);
  // bbox_min_z = pull(result_vector)[0];
  // region_props['bbox_min_z'] = bbox_min_z;

  // tier1::crop(device, max_per_label, result_vector, offset, 11, 0);
  // bbox_max_x = pull(result_vector)[0];
  // region_props['bbox_max_x'] = bbox_max_x;

  // tier1::crop(device, max_per_label, result_vector, offset, 13, 0);
  // bbox_max_y = pull(result_vector)[0];
  // region_props['bbox_max_y'] = bbox_max_y;

  // tier1::crop(device, max_per_label, result_vector, offset, 15, 0);
  // bbox_max_z = pull(result_vector)[0];
  // region_props['bbox_max_z'] = bbox_max_z;
}


} // namespace cle

#endif // __INCLUDE_STATISTICS_HPP
