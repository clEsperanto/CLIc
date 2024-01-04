#ifndef __INCLUDE_TIER3_HPP
#define __INCLUDE_TIER3_HPP

#include "array.hpp"
#include "device.hpp"
#include "execution.hpp"

/**
 * @namespace cle::tier3
 * @brief Contains all the functions of the tier 3 category.
 *
 */
namespace cle::tier3
{


auto
bounding_box_func(const Device::Pointer & device, const Array::Pointer & src) -> std::array<float, 6>;

auto
center_of_mass_func(const Device::Pointer & device, const Array::Pointer & src) -> std::array<float, 3>;

// proximal_other_labels_count

auto
exclude_labels_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    const Array::Pointer &  list,
                    Array::Pointer          dst) -> Array::Pointer;

auto
exclude_labels_on_edges_func(const Device::Pointer & device,
                             const Array::Pointer &  src,
                             Array::Pointer          dst,
                             bool                    exclude_x,
                             bool                    exclude_y,
                             bool                    exclude_z) -> Array::Pointer;

// exclude_labels_with_values_equal_to_constant
// exclude_labels_with_values_not_equal_to_constant
// exclude_labels_outside_size_range
// exclude_labels_with_values_out_of_range
// exclude_labels_with_values_within_range

auto
flag_existing_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

auto
gamma_correction_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float gamma)
  -> Array::Pointer;

auto
generate_binary_overlap_matrix_func(const Device::Pointer & device,
                                    const Array::Pointer &  src0,
                                    const Array::Pointer &  src1,
                                    Array::Pointer          dst) -> Array::Pointer;

auto
generate_touch_matrix_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

// generate_n_nearest_neighbors_matrix
// generate_proximal_neighbors_matrix
// generate_touch_count_matrix
// generate_touch_mean_intensity_matrix

auto
histogram_func(const Device::Pointer & device,
               const Array::Pointer &  src,
               Array::Pointer          dst,
               int                     nbins,
               float                   min = NaN,
               float                   max = NaN) -> Array::Pointer;

auto
jaccard_index_func(const Device::Pointer & device, const Array::Pointer & src1, const Array::Pointer & src2) -> float;

auto
labelled_spots_to_pointlist_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

// maximum_of_n_most_touching_neighbors_map
// maximum_of_n_nearest_neighbors_map
// maximum_of_touch_portion_within_range_neighbors_map
// maximum_of_touching_neighbors_map
// maximum_of_proximal_neighbors_map

auto
maximum_position_func(const Device::Pointer & device, const Array::Pointer & src) -> std::array<size_t, 3>;

auto
mean_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float;

// mean_of_n_most_touching_neighbors_map
// mean_of_n_nearest_neighbors_map
// mean_of_proximal_neighbors_map
// mean_of_touch_portion_within_range_neighbors_map
// mean_of_touching_neighbors_map

// minimum_of_n_most_touching_neighbors_map
// minimum_of_n_nearest_neighbors_map
// minimum_of_proximal_neighbors_map
// minimum_of_touch_portion_within_range_neighbors_map
// minimum_of_touching_neighbors_map

auto
minimum_position_func(const Device::Pointer & device, const Array::Pointer & src) -> std::array<size_t, 3>;

// mode_of_n_most_touching_neighbors_map
// mode_of_n_nearest_neighbors_map
// mode_of_proximal_neighbors_map
// mode_of_touch_portion_within_range_neighbors_map
// mode_of_touching_neighbors_map


// standard_deviation_of_n_most_touching_neighbors_map
// standard_deviation_of_n_nearest_neighbors_map
// standard_deviation_of_touch_portion_within_range_neighbors_map
// standard_deviation_of_touching_neighbors_map
// standard_deviation_of_proximal_neighbors_map

// z_position_range_projection

} // namespace cle::tier3

#endif // __INCLUDE_TIER3_HPP
