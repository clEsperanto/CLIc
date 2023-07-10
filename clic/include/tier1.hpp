#ifndef __INCLUDE_TIER1_HPP
#define __INCLUDE_TIER1_HPP

#include "tier0.hpp"

namespace cle::tier1
{

auto
absolute_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

auto
add_images_weighted_func(const Device::Pointer & device,
                         const Array::Pointer &  src0,
                         const Array::Pointer &  src1,
                         Array::Pointer          dst,
                         const float &           factor0,
                         const float &           factor1) -> Array::Pointer;

auto
add_image_and_scalar_func(const Device::Pointer & device,
                          const Array::Pointer &  src,
                          Array::Pointer          dst,
                          const float &           scalar) -> Array::Pointer;

// average_distance_of_n_far_off_distances_func
// average_distance_of_n_far_off_distances_func
// average_distance_of_n_shortest_distances_func
// average_distance_of_n_shortest_distances_func
// average_distance_of_n_nearest_distances_func
// average_distance_of_touching_neighbors_func
auto
binary_and_func(const Device::Pointer & device,
                const Array::Pointer &  src0,
                const Array::Pointer &  src1,
                Array::Pointer          dst) -> Array::Pointer;

auto
binary_edge_detection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

auto
binary_not_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

auto
binary_or_func(const Device::Pointer & device,
               const Array::Pointer &  src0,
               const Array::Pointer &  src1,
               Array::Pointer          dst) -> Array::Pointer;

auto
binary_subtract_func(const Device::Pointer & device,
                     const Array::Pointer &  src0,
                     const Array::Pointer &  src1,
                     Array::Pointer          dst) -> Array::Pointer;

auto
binary_xor_func(const Device::Pointer & device,
                const Array::Pointer &  src0,
                const Array::Pointer &  src1,
                Array::Pointer          dst) -> Array::Pointer;

auto
convolve_func(const Device::Pointer & device,
              const Array::Pointer &  src0,
              const Array::Pointer &  src1,
              Array::Pointer          dst) -> Array::Pointer;

auto
copy_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

// copy_slice_func
// copy_horizontal_slice_func
// copy_vertical_slice_func
// count_touching_neighbors_func
// crop_func
// cubic_root_func
// detect_label_edges_func
// detect_maxima_box_func
// detect_minima_box_func
auto
dilate_box_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

// dilate_box_slice_by_slice_func
auto
dilate_sphere_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

// dilate_sphere_slice_by_slice_func
auto
divide_images_func(const Device::Pointer & device,
                   const Array::Pointer &  src0,
                   const Array::Pointer &  src1,
                   Array::Pointer          dst) -> Array::Pointer;

// divide_scalar_by_image_func
// draw_box_func
// draw_sphere_func
// draw_line_func
// downsample_slice_by_slice_half_median_func
auto
equal_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer;

auto
equal_constant_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    const float &           scalar) -> Array::Pointer;

auto
erode_box_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

// erode_box_slice_by_slice_func
auto
erode_sphere_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

// erode_sphere_slice_by_slice_func
auto
exponential_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

// flip_func
auto
gaussian_blur_func(const Device::Pointer & device,
                   const Array::Pointer &  src,
                   Array::Pointer          dst,
                   const float &           sigma_x,
                   const float &           sigma_y,
                   const float &           sigma_z) -> Array::Pointer;

// generate_angle_matrix_func
// generate_binary_overlap_matrix_func
// generate_distance_matrix_func
// generate_touch_matrix_func
auto
gradient_x_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

auto
gradient_y_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

auto
gradient_z_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

auto
greater_func(const Device::Pointer & device,
             const Array::Pointer &  src0,
             const Array::Pointer &  src1,
             Array::Pointer          dst) -> Array::Pointer;

auto
greater_constant_func(const Device::Pointer & device,
                      const Array::Pointer &  src,
                      Array::Pointer          dst,
                      const float &           scalar) -> Array::Pointer;

auto
greater_or_equal_func(const Device::Pointer & device,
                      const Array::Pointer &  src0,
                      const Array::Pointer &  src1,
                      Array::Pointer          dst) -> Array::Pointer;

auto
greater_or_equal_constant_func(const Device::Pointer & device,
                               const Array::Pointer &  src,
                               Array::Pointer          dst,
                               const float &           scalar) -> Array::Pointer;

// hessian_eigenvalues_func
auto
laplace_box_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

auto
laplace_diamond_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

auto
logarithm_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

auto
mask_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, const Array::Pointer & mask)
  -> Array::Pointer;

auto
mask_label_func(const Device::Pointer & device,
                const Array::Pointer &  src0,
                const Array::Pointer &  src1,
                Array::Pointer          dst,
                const float &           label) -> Array::Pointer;

auto
maximum_image_and_scalar_func(const Device::Pointer & device,
                              const Array::Pointer &  src,
                              Array::Pointer          dst,
                              const float &           scalar) -> Array::Pointer;

auto
maximum_images_func(const Device::Pointer & device,
                    const Array::Pointer &  src0,
                    const Array::Pointer &  src1,
                    Array::Pointer          dst) -> Array::Pointer;

auto
maximum_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 const int &             radius_x,
                 const int &             radius_y,
                 const int &             radius_z) -> Array::Pointer;

// maximum_distance_of_touching_neighbors_func
// maximum_distance_of_n_shortest_distances_func
auto
maximum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

auto
maximum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

auto
maximum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

auto
mean_box_func(const Device::Pointer & device,
              const Array::Pointer &  src,
              Array::Pointer          dst,
              const int &             radius_x,
              const int &             radius_y,
              const int &             radius_z) -> Array::Pointer;

auto
mean_sphere_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 const int &             radius_x,
                 const int &             radius_y,
                 const int &             radius_z) -> Array::Pointer;

auto
mean_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

auto
mean_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

auto
mean_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

// median_box_func
// median_sphere_func
auto
minimum_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 const int &             radius_x,
                 const int &             radius_y,
                 const int &             radius_z) -> Array::Pointer;

// minimum_distance_of_touching_neighbors_func
auto
minimum_image_and_scalar_func(const Device::Pointer & device,
                              const Array::Pointer &  src,
                              Array::Pointer          dst,
                              const float &           scalar) -> Array::Pointer;

auto
minimum_images_func(const Device::Pointer & device,
                    const Array::Pointer &  src0,
                    const Array::Pointer &  src1,
                    Array::Pointer          dst) -> Array::Pointer;

auto
minimum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

auto
minimum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

auto
minimum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

// mode_box_func
// mode_sphere_func
// modulo_images_func
// multiply_image_and_coordinate_func
auto
multiply_image_and_scalar_func(const Device::Pointer & device,
                               const Array::Pointer &  src,
                               Array::Pointer          dst,
                               const float &           scalar) -> Array::Pointer;

auto
multiply_images_func(const Device::Pointer & device,
                     const Array::Pointer &  src0,
                     const Array::Pointer &  src1,
                     Array::Pointer          dst) -> Array::Pointer;

// n_closest_points_func
// nan_to_num_func
auto
nonzero_maximum_box_func(const Device::Pointer & device,
                         const Array::Pointer &  src,
                         Array::Pointer          dst0,
                         Array::Pointer          dst1) -> Array::Pointer;

auto
nonzero_maximum_diamond_func(const Device::Pointer & device,
                             const Array::Pointer &  src,
                             Array::Pointer          dst0,
                             Array::Pointer          dst1) -> Array::Pointer;

auto
nonzero_minimum_box_func(const Device::Pointer & device,
                         const Array::Pointer &  src,
                         Array::Pointer          dst0,
                         Array::Pointer          dst1) -> Array::Pointer;

auto
nonzero_minimum_diamond_func(const Device::Pointer & device,
                             const Array::Pointer &  src,
                             Array::Pointer          dst0,
                             Array::Pointer          dst1) -> Array::Pointer;

auto
not_equal_func(const Device::Pointer & device,
               const Array::Pointer &  src0,
               const Array::Pointer &  src1,
               Array::Pointer          dst) -> Array::Pointer;

auto
not_equal_constant_func(const Device::Pointer & device,
                        const Array::Pointer &  src,
                        Array::Pointer          dst,
                        const float &           scalar) -> Array::Pointer;

// paste_func
auto
onlyzero_overwrite_maximum_box_func(const Device::Pointer & device,
                                    const Array::Pointer &  src,
                                    Array::Pointer          dst0,
                                    Array::Pointer          dst1) -> Array::Pointer;

auto
onlyzero_overwrite_maximum_diamond_func(const Device::Pointer & device,
                                        const Array::Pointer &  src,
                                        Array::Pointer          dst0,
                                        Array::Pointer          dst1) -> Array::Pointer;

// point_index_list_to_mesh_func
// point_index_list_to_touch_matrix_func
auto
power_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, const float & scalar)
  -> Array::Pointer;

auto
power_images_func(const Device::Pointer & device,
                  const Array::Pointer &  src0,
                  const Array::Pointer &  src1,
                  Array::Pointer          dst) -> Array::Pointer;

// range_func
// read_intensities_from_map_func
// read_intensities_from_positions_func
auto
replace_intensities_func(const Device::Pointer & device,
                         const Array::Pointer &  src0,
                         const Array::Pointer &  src1,
                         Array::Pointer          dst) -> Array::Pointer;

auto
replace_intensity_func(const Device::Pointer & device,
                       const Array::Pointer &  src,
                       Array::Pointer          dst,
                       const float &           scalar0,
                       const float &           scalar1) -> Array::Pointer;

// resample_func
// touch_matrix_to_mesh_func
auto
maximum_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    const int &             radius_x,
                    const int &             radius_y,
                    const int &             radius_z) -> Array::Pointer;

auto
minimum_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    const int &             radius_x,
                    const int &             radius_y,
                    const int &             radius_z) -> Array::Pointer;

// multiply_matrix_func
// reciprocal_func
auto
set_func(const Device::Pointer & device, Array::Pointer dst, const float & scalar) -> Array::Pointer;

auto
set_column_func(const Device::Pointer & device, Array::Pointer dst, const int & column, const float & value)
  -> Array::Pointer;

// set_image_borders_func
// set_plane_func
// set_ramp_x_func
// set_ramp_y_func
// set_ramp_z_func
auto
set_row_func(const Device::Pointer & device, Array::Pointer dst, const int & row, const float & value)
  -> Array::Pointer;

auto
set_nonzero_pixels_to_pixelindex_func(const Device::Pointer & device,
                                      const Array::Pointer &  src,
                                      Array::Pointer          dst,
                                      const int &             offset) -> Array::Pointer;

// set_where_x_equals_y_func
// set_where_x_greater_than_y_func
// set_where_x_smaller_than_y_func
// sign_func
auto
smaller_func(const Device::Pointer & device,
             const Array::Pointer &  src0,
             const Array::Pointer &  src1,
             Array::Pointer          dst) -> Array::Pointer;

auto
smaller_constant_func(const Device::Pointer & device,
                      const Array::Pointer &  src,
                      Array::Pointer          dst,
                      const float &           scalar) -> Array::Pointer;

auto
smaller_or_equal_func(const Device::Pointer & device,
                      const Array::Pointer &  src0,
                      const Array::Pointer &  src1,
                      Array::Pointer          dst) -> Array::Pointer;

auto
smaller_or_equal_constant_func(const Device::Pointer & device,
                               const Array::Pointer &  src,
                               Array::Pointer          dst,
                               const float &           scalar) -> Array::Pointer;

auto
sobel_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

// square_root_func
// standard_deviation_z_projection_func
auto
subtract_image_from_scalar_func(const Device::Pointer & device,
                                const Array::Pointer &  src,
                                Array::Pointer          dst,
                                const float &           scalar) -> Array::Pointer;

auto
sum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

auto
sum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

auto
sum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


// transpose_xy_func
// transpose_xz_func
// transpose_yz_func
// undefined_to_zero_func
// variance_box_func
// variance_sphere_func
// write_values_to_positions_func

} // namespace cle::tier1

#endif // __INCLUDE_TIER1_HPP
