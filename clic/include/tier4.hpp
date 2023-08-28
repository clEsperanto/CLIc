#ifndef __INCLUDE_TIER4_HPP
#define __INCLUDE_TIER4_HPP

#include "array.hpp"
#include "device.hpp"
#include "execution.hpp"

namespace cle::tier4
{

// auto local_cross_correlation_func
// auto proximal_other_labels_count_map_func
// auto erode_labels_func
// auto erode_connected_labels_func
// auto exclude_labels_with_average_values_out_of_range_func
// auto exclude_labels_with_average_values_within_range_func
// auto exclude_labels_with_map_values_equal_to_constant_func
// auto exclude_labels_with_map_values_not_equal_to_constant_func
// auto exclude_labels_with_map_values_out_of_range_func
// auto exclude_labels_with_map_values_within_range_func
// auto exclude_large_labels_func
// auto exclude_small_labels_func
// auto extend_labeling_via_voronoi_func
// auto dilate_labels_func
// auto extended_depth_of_focus_variance_projection_func
// auto generate_n_most_touching_neighbors_matrix_func
// auto generate_touch_portion_matrix_func
// auto generate_touch_portion_within_range_neighbors_matrix_func

auto
mean_squared_error_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1)
  -> float;

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
// auto spots_to_pointlist_func
// auto subtract_labels_func
// auto touch_portion_within_range_neighbor_count_func
// auto touch_portion_within_range_neighbor_count_map_func
// auto touching_neighbor_count_map_func

auto
relabel_sequential_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int blocksize)
  -> Array::Pointer;

auto
threshold_otsu_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

} // namespace cle::tier4

#endif // __INCLUDE_TIER4_HPP
