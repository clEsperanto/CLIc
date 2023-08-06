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
                         float                   factor0,
                         float                   factor1) -> Array::Pointer;

auto
add_image_and_scalar_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar)
  -> Array::Pointer;

// @StRigaud TODO : average_distance_of_n_far_off_distances_func
// @StRigaud TODO : average_distance_of_n_far_off_distances_func
// @StRigaud TODO : average_distance_of_n_shortest_distances_func
// @StRigaud TODO : average_distance_of_n_shortest_distances_func
// @StRigaud TODO : average_distance_of_n_nearest_distances_func
// @StRigaud TODO : average_distance_of_touching_neighbors_func

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
block_enumerate_func(const Device::Pointer & device,
                     const Array::Pointer &  src0,
                     const Array::Pointer &  src1,
                     Array::Pointer          dst,
                     int                     blocksize) -> Array::Pointer;

auto
convolve_func(const Device::Pointer & device,
              const Array::Pointer &  src0,
              const Array::Pointer &  src1,
              Array::Pointer          dst) -> Array::Pointer;

auto
copy_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

auto
copy_slice_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int slice)
  -> Array::Pointer;

auto
copy_horizontal_slice_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int slice)
  -> Array::Pointer;

auto
copy_vertical_slice_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int slice)
  -> Array::Pointer;

// @StRigaud TODO : count_touching_neighbors_func

auto
crop_func(const Device::Pointer & device,
          const Array::Pointer &  src,
          Array::Pointer          dst,
          int                     start_x,
          int                     start_y,
          int                     start_z,
          int                     width,
          int                     height,
          int                     depth) -> Array::Pointer;

auto
cubic_root_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

auto
detect_label_edges_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

auto
detect_maxima_box_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

auto
detect_minima_box_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


auto
dilate_box_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

// @StRigaud TODO : dilate_box_slice_by_slice_func

auto
dilate_sphere_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

// @StRigaud TODO : dilate_sphere_slice_by_slice_func

auto
divide_images_func(const Device::Pointer & device,
                   const Array::Pointer &  src0,
                   const Array::Pointer &  src1,
                   Array::Pointer          dst) -> Array::Pointer;

auto
divide_image_and_scalar_func(const Device::Pointer & device,
                             const Array::Pointer &  src,
                             Array::Pointer          dst,
                             float                   scalar) -> Array::Pointer;

// @StRigaud TODO : draw_box_func
// @StRigaud TODO : draw_sphere_func
// @StRigaud TODO : draw_line_func
// @StRigaud TODO : downsample_slice_by_slice_half_median_func

auto
equal_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer;

auto
equal_constant_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar)
  -> Array::Pointer;

auto
erode_box_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

// @StRigaud TODO : erode_box_slice_by_slice_func

auto
erode_sphere_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

// @StRigaud TODO : erode_sphere_slice_by_slice_func

auto
exponential_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

auto
flip_func(const Device::Pointer & device,
          const Array::Pointer &  src,
          Array::Pointer          dst,
          bool                    flip_x,
          bool                    flip_y,
          bool                    flip_z) -> Array::Pointer;

auto
gaussian_blur_func(const Device::Pointer & device,
                   const Array::Pointer &  src,
                   Array::Pointer          dst,
                   float                   sigma_x,
                   float                   sigma_y,
                   float                   sigma_z) -> Array::Pointer;

// @StRigaud TODO : generate_angle_matrix_func
// @StRigaud TODO : generate_binary_overlap_matrix_func
// @StRigaud TODO : generate_distance_matrix_func
// @StRigaud TODO : generate_touch_matrix_func

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
greater_constant_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar)
  -> Array::Pointer;

auto
greater_or_equal_func(const Device::Pointer & device,
                      const Array::Pointer &  src0,
                      const Array::Pointer &  src1,
                      Array::Pointer          dst) -> Array::Pointer;

auto
greater_or_equal_constant_func(const Device::Pointer & device,
                               const Array::Pointer &  src,
                               Array::Pointer          dst,
                               float                   scalar) -> Array::Pointer;

// @StRigaud TODO : hessian_eigenvalues_func

auto
laplace_box_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

auto
laplace_diamond_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

auto
logarithm_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

auto
mask_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & mask, Array::Pointer dst)
  -> Array::Pointer;

auto
mask_label_func(const Device::Pointer & device,
                const Array::Pointer &  src0,
                const Array::Pointer &  src1,
                Array::Pointer          dst,
                float                   label) -> Array::Pointer;

auto
maximum_image_and_scalar_func(const Device::Pointer & device,
                              const Array::Pointer &  src,
                              Array::Pointer          dst,
                              float                   scalar) -> Array::Pointer;

auto
maximum_images_func(const Device::Pointer & device,
                    const Array::Pointer &  src0,
                    const Array::Pointer &  src1,
                    Array::Pointer          dst) -> Array::Pointer;

auto
maximum_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 int                     radius_x,
                 int                     radius_y,
                 int                     radius_z) -> Array::Pointer;

// @StRigaud TODO : maximum_distance_of_touching_neighbors_func
// @StRigaud TODO : maximum_distance_of_n_shortest_distances_func

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
              int                     radius_x,
              int                     radius_y,
              int                     radius_z) -> Array::Pointer;

auto
mean_sphere_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 int                     radius_x,
                 int                     radius_y,
                 int                     radius_z) -> Array::Pointer;

auto
mean_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

auto
mean_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

auto
mean_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

auto
median_box_func(const Device::Pointer & device,
                const Array::Pointer &  src,
                Array::Pointer          dst,
                int                     radius_x,
                int                     radius_y,
                int                     radius_z) -> Array::Pointer;

auto
median_sphere_func(const Device::Pointer & device,
                   const Array::Pointer &  src,
                   Array::Pointer          dst,
                   int                     radius_x,
                   int                     radius_y,
                   int                     radius_z) -> Array::Pointer;

auto
minimum_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 int                     radius_x,
                 int                     radius_y,
                 int                     radius_z) -> Array::Pointer;

// @StRigaud TODO : minimum_distance_of_touching_neighbors_func

auto
minimum_image_and_scalar_func(const Device::Pointer & device,
                              const Array::Pointer &  src,
                              Array::Pointer          dst,
                              float                   scalar) -> Array::Pointer;

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

auto
minimum_of_masked_pixels_reduction_func(const Device::Pointer & device,
                                        const Array::Pointer &  src,
                                        const Array::Pointer &  mask,
                                        Array::Pointer          reduced_src,
                                        Array::Pointer          reduced_mask) -> Array::Pointer;

auto
mode_box_func(const Device::Pointer & device,
              const Array::Pointer &  src,
              Array::Pointer          dst,
              int                     radius_x,
              int                     radius_y,
              int                     radius_z) -> Array::Pointer;

auto
mode_sphere_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 int                     radius_x,
                 int                     radius_y,
                 int                     radius_z) -> Array::Pointer;

auto
modulo_images_func(const Device::Pointer & device,
                   const Array::Pointer &  src0,
                   const Array::Pointer &  src1,
                   Array::Pointer          dst) -> Array::Pointer;

auto
multiply_image_and_coordinate_func(const Device::Pointer & device,
                                   const Array::Pointer &  src,
                                   Array::Pointer          dst,
                                   int                     dimension) -> Array::Pointer;

auto
multiply_image_and_scalar_func(const Device::Pointer & device,
                               const Array::Pointer &  src,
                               Array::Pointer          dst,
                               float                   scalar) -> Array::Pointer;

auto
multiply_images_func(const Device::Pointer & device,
                     const Array::Pointer &  src0,
                     const Array::Pointer &  src1,
                     Array::Pointer          dst) -> Array::Pointer;

// @StRigaud TODO : n_closest_points_func

auto
nan_to_num_func(const Device::Pointer & device,
                const Array::Pointer &  src,
                Array::Pointer          dst,
                float                   nan = NaN,
                float                   posinf = pINF,
                float                   neginf = nINF) -> Array::Pointer;

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
not_equal_constant_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar)
  -> Array::Pointer;

auto
paste_func(const Device::Pointer & device,
           const Array::Pointer &  src,
           Array::Pointer          dst,
           int                     index_x,
           int                     index_y,
           int                     index_z) -> Array::Pointer;

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

// @StRigaud TODO : point_index_list_to_mesh_func
// @StRigaud TODO : point_index_list_to_touch_matrix_func

auto
power_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar)
  -> Array::Pointer;

auto
power_images_func(const Device::Pointer & device,
                  const Array::Pointer &  src0,
                  const Array::Pointer &  src1,
                  Array::Pointer          dst) -> Array::Pointer;

auto
range_func(const Device::Pointer & device,
           const Array::Pointer &  src,
           Array::Pointer          dst,
           int                     start_x,
           int                     stop_x,
           int                     step_x,
           int                     start_y,
           int                     stop_y,
           int                     step_y,
           int                     start_z,
           int                     stop_z,
           int                     step_z) -> Array::Pointer;

// @StRigaud TODO : read_intensities_from_map_func

auto
read_intensities_from_positions_func(const Device::Pointer & device,
                                     const Array::Pointer &  src,
                                     const Array::Pointer &  list,
                                     Array::Pointer          dst) -> Array::Pointer;

auto
replace_intensities_func(const Device::Pointer & device,
                         const Array::Pointer &  src0,
                         const Array::Pointer &  src1,
                         Array::Pointer          dst) -> Array::Pointer;

auto
replace_intensity_func(const Device::Pointer & device,
                       const Array::Pointer &  src,
                       Array::Pointer          dst,
                       float                   scalar0,
                       float                   scalar1) -> Array::Pointer;

// @StRigaud TODO : resample_func
// @StRigaud TODO : touch_matrix_to_mesh_func

auto
maximum_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    int                     radius_x,
                    int                     radius_y,
                    int                     radius_z) -> Array::Pointer;

auto
minimum_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    int                     radius_x,
                    int                     radius_y,
                    int                     radius_z) -> Array::Pointer;

auto
multiply_matrix_func(const Device::Pointer & device,
                     const Array::Pointer &  src0,
                     const Array::Pointer &  src1,
                     Array::Pointer          dst) -> Array::Pointer;

auto
reciprocal_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

auto
set_func(const Device::Pointer & device, const Array::Pointer & src, float scalar) -> Array::Pointer;

auto
set_column_func(const Device::Pointer & device, const Array::Pointer & src, int column, float value) -> Array::Pointer;

auto
set_image_borders_func(const Device::Pointer & device, const Array::Pointer & src, float value) -> Array::Pointer;

auto
set_plane_func(const Device::Pointer & device, const Array::Pointer & src, int plane, float value) -> Array::Pointer;

auto
set_ramp_x_func(const Device::Pointer & device, const Array::Pointer & src) -> Array::Pointer;

auto
set_ramp_y_func(const Device::Pointer & device, const Array::Pointer & src) -> Array::Pointer;

auto
set_ramp_z_func(const Device::Pointer & device, const Array::Pointer & src) -> Array::Pointer;

auto
set_row_func(const Device::Pointer & device, const Array::Pointer & src, int row, float value) -> Array::Pointer;

auto
set_nonzero_pixels_to_pixelindex_func(const Device::Pointer & device,
                                      const Array::Pointer &  src,
                                      Array::Pointer          dst,
                                      int                     offset) -> Array::Pointer;

auto
set_where_x_equals_y_func(const Device::Pointer & device, const Array::Pointer & src, float value) -> Array::Pointer;

auto
set_where_x_greater_than_y_func(const Device::Pointer & device, const Array::Pointer & src, float value)
  -> Array::Pointer;
auto
set_where_x_smaller_than_y_func(const Device::Pointer & device, const Array::Pointer & src, float value)
  -> Array::Pointer;

auto
sign_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

auto
smaller_func(const Device::Pointer & device,
             const Array::Pointer &  src0,
             const Array::Pointer &  src1,
             Array::Pointer          dst) -> Array::Pointer;

auto
smaller_constant_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar)
  -> Array::Pointer;

auto
smaller_or_equal_func(const Device::Pointer & device,
                      const Array::Pointer &  src0,
                      const Array::Pointer &  src1,
                      Array::Pointer          dst) -> Array::Pointer;

auto
smaller_or_equal_constant_func(const Device::Pointer & device,
                               const Array::Pointer &  src,
                               Array::Pointer          dst,
                               float                   scalar) -> Array::Pointer;

auto
sobel_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

auto
square_root_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

// @StRigaud TODO : standard_deviation_z_projection_func

auto
subtract_image_from_scalar_func(const Device::Pointer & device,
                                const Array::Pointer &  src,
                                Array::Pointer          dst,
                                float                   scalar) -> Array::Pointer;

auto
sum_reduction_x_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int blocksize)
  -> Array::Pointer;

auto
sum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

auto
sum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

auto
sum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

auto
transpose_xy_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

auto
transpose_xz_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

auto
transpose_yz_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

auto
undefined_to_zero_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

auto
variance_box_func(const Device::Pointer & device,
                  const Array::Pointer &  src,
                  Array::Pointer          dst,
                  int                     radius_x,
                  int                     radius_y,
                  int                     radius_z) -> Array::Pointer;

auto
variance_sphere_func(const Device::Pointer & device,
                     const Array::Pointer &  src,
                     Array::Pointer          dst,
                     int                     radius_x,
                     int                     radius_y,
                     int                     radius_z) -> Array::Pointer;

auto
write_values_to_positions_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

} // namespace cle::tier1

#endif // __INCLUDE_TIER1_HPP
