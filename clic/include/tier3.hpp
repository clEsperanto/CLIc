#ifndef __INCLUDE_TIER3_HPP
#define __INCLUDE_TIER3_HPP

#include "array.hpp"
#include "device.hpp"
#include "execution.hpp"

#include <limits>

namespace cle::tier3
{


// auto bounding_box_func
// auto center_of_mass_func
// auto proximal_other_labels_count_func
// auto divide_by_gaussian_background_func
// auto exclude_labels_func
// auto exclude_labels_on_edges_func
// auto exclude_labels_with_values_equal_to_constant_func
// auto exclude_labels_with_values_not_equal_to_constant_func
// auto exclude_labels_outside_size_range_func
// auto exclude_labels_with_values_out_of_range_func
// auto exclude_labels_with_values_within_range_func

auto
flag_existing_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

auto
gamma_correction_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float gamma)
  -> Array::Pointer;

// auto generate_n_nearest_neighbors_matrix_func
// auto generate_proximal_neighbors_matrix_func
// auto generate_touch_count_matrix_func
// auto generate_touch_mean_intensity_matrix_func

auto
histogram_func(const Device::Pointer & device,
               const Array::Pointer &  src,
               Array::Pointer          dst,
               int                     nbins,
               float                   min = std::numeric_limits<float>::quiet_NaN(),
               float                   max = std::numeric_limits<float>::quiet_NaN()) -> Array::Pointer;

// auto jaccard_index_func
// auto labelled_spots_to_pointlist_func
// auto maximum_of_n_most_touching_neighbors_map_func
// auto maximum_of_n_nearest_neighbors_map_func
// auto maximum_of_touch_portion_within_range_neighbors_map_func
// auto maximum_of_touching_neighbors_map_func
// auto maximum_of_proximal_neighbors_map_func
// auto maximum_position_func

auto
mean_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float;

// auto mean_of_n_most_touching_neighbors_map_func
// auto mean_of_n_nearest_neighbors_map_func
// auto mean_of_proximal_neighbors_map_func
// auto mean_of_touch_portion_within_range_neighbors_map_func
// auto mean_of_touching_neighbors_map_func
// auto minimum_of_n_most_touching_neighbors_map_func
// auto minimum_of_n_nearest_neighbors_map_func
// auto minimum_of_proximal_neighbors_map_func
// auto minimum_of_touch_portion_within_range_neighbors_map_func
// auto minimum_of_touching_neighbors_map_func
// auto minimum_position_func
// auto mode_of_n_most_touching_neighbors_map_func
// auto mode_of_n_nearest_neighbors_map_func
// auto mode_of_proximal_neighbors_map_func
// auto mode_of_touch_portion_within_range_neighbors_map_func
// auto mode_of_touching_neighbors_map_func


// auto standard_deviation_of_n_most_touching_neighbors_map_func
// auto standard_deviation_of_n_nearest_neighbors_map_func
// auto standard_deviation_of_touch_portion_within_range_neighbors_map_func
// auto standard_deviation_of_touching_neighbors_map_func
// auto standard_deviation_of_proximal_neighbors_map_func
// auto subtract_gaussian_background_func
// auto z_position_range_projection_func

} // namespace cle::tier3

#endif // __INCLUDE_TIER3_HPP
