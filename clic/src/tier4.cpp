#include "tier4.hpp"
#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"

#include <numeric>

namespace cle::tier4
{

// auto proximal_other_labels_count_map_func

// auto erode_connected_labels_func
// auto exclude_labels_with_average_values_out_of_range_func
// auto exclude_labels_with_average_values_within_range_func
// auto exclude_labels_with_map_values_equal_to_constant_func
// auto exclude_labels_with_map_values_not_equal_to_constant_func
// auto exclude_labels_with_map_values_out_of_range_func
// auto exclude_labels_with_map_values_within_range_func
// auto exclude_large_labels_func
// auto exclude_small_labels_func

// auto extended_depth_of_focus_variance_projection_func
// auto generate_n_most_touching_neighbors_matrix_func
// auto generate_touch_portion_matrix_func
// auto generate_touch_portion_within_range_neighbors_matrix_func

auto
mean_squared_error_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1)
  -> float
{
  auto temp = tier2::squared_difference_func(device, src0, src1, nullptr);
  return tier3::mean_of_all_pixels_func(device, temp);
}

// auto label_nonzero_pixel_count_map_func
// auto label_nonzero_pixel_count_ratio_map_func
// auto label_overlap_count_map_func
// auto local_maximum_touching_neighbor_count_map_func
// auto local_mean_touching_neighbor_count_map_func
// auto local_median_touching_neighbor_count_map_func
// auto local_minimum_touching_neighbor_count_map_func
// auto local_standard_deviation_touching_neighbor_count_map_func
// auto merge_touching_labels_func
// auto proximal_neighbor_count_func
// auto proximal_neighbor_count_map_func
// auto sorensen_dice_coefficient_func

auto
spots_to_pointlist_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  auto labeled_spots = tier2::label_spots_func(device, src, nullptr);
  return tier3::labelled_spots_to_pointlist_func(device, labeled_spots, dst);
}

// auto subtract_labels_func
// auto touch_portion_within_range_neighbor_count_func
// auto touch_portion_within_range_neighbor_count_map_func
// auto touching_neighbor_count_map_func

auto
relabel_sequential_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int blocksize)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  auto max_label = static_cast<int>(tier2::maximum_of_all_pixels_func(device, src));
  auto flagged = Array::create(int(max_label + 1), 1, 1, 1, src->dtype(), src->mtype(), src->device());
  flagged->fill(0);
  tier3::flag_existing_labels_func(device, src, flagged);
  tier1::set_column_func(device, flagged, 0, 0);
  auto block_sums =
    Array::create(((max_label + 1) / blocksize) + 1, 1, 1, 1, flagged->dtype(), flagged->mtype(), flagged->device());
  tier1::sum_reduction_x_func(device, flagged, block_sums, blocksize);
  auto new_indices = Array::create(max_label + 1, 1, 1, 1, flagged->dtype(), flagged->mtype(), flagged->device());
  tier1::block_enumerate_func(device, flagged, block_sums, new_indices, blocksize);
  tier1::replace_values_func(device, src, new_indices, dst);
  return dst;
}

auto
threshold_otsu_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  constexpr int bin = 256;
  const float   min_intensity = tier2::minimum_of_all_pixels_func(device, src);
  const float   max_intensity = tier2::maximum_of_all_pixels_func(device, src);
  auto          hist_array = Array::create(bin, 1, 1, 1, dType::FLOAT, mType::BUFFER, src->device());
  tier3::histogram_func(device, src, hist_array, bin, min_intensity, max_intensity);
  std::vector<float> histogram_array(hist_array->size());
  hist_array->read(histogram_array.data());
  float              threshold = -1;
  float              max_variance = -1;
  float              variance = 0;
  float              sum_1 = 0;
  float              sum_2 = 0;
  float              weight_1 = 0;
  float              weight_2 = 0;
  float              mean_1 = 0;
  float              mean_2 = 0;
  const float        nb_pixels = src->size();
  const float        intensity_factor = (max_intensity - min_intensity) / (bin - 1);
  std::vector<float> range(histogram_array.size());
  std::iota(range.begin(), range.end(), 0.0f);
  std::transform(range.begin(), range.end(), range.begin(), [intensity_factor, min_intensity](float intensity) {
    return intensity * intensity_factor + min_intensity;
  });
  sum_1 = std::transform_reduce(
    range.begin(), range.end(), histogram_array.begin(), 0.0f, std::plus<>(), [](float intensity, float hist_value) {
      return intensity * hist_value;
    });
  for (size_t index = 0; index < range.size(); ++index)
  {
    if (histogram_array[index] == 0)
    {
      continue;
    }
    weight_1 += histogram_array[index];
    weight_2 = nb_pixels - weight_1;
    sum_2 += range[index] * histogram_array[index];
    mean_1 = sum_2 / weight_1;
    mean_2 = (sum_1 - sum_2) / weight_2;
    variance = weight_1 * weight_2 * ((mean_1 - mean_2) * (mean_1 - mean_2));
    if (variance > max_variance)
    {
      threshold = range[index];
      max_variance = variance;
    }
  }
  tier0::create_like(src, dst, dType::UINT8);
  return tier1::greater_constant_func(device, src, dst, threshold);
}


} // namespace cle::tier4
