#ifndef __INCLUDE_TIER2_HPP
#define __INCLUDE_TIER2_HPP

#include "array.hpp"
#include "device.hpp"
#include "execution.hpp"


namespace cle::tier2
{

auto
add_images_func(const Device::Pointer & device,
                const Array::Pointer &  src0,
                const Array::Pointer &  src1,
                Array::Pointer          dst) -> Array::Pointer;

auto
bottom_hat_box_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    const int &             radius_x,
                    const int &             radius_y,
                    const int &             radius_z) -> Array::Pointer;

auto
bottom_hat_sphere_func(const Device::Pointer & device,
                       const Array::Pointer &  src,
                       Array::Pointer          dst,
                       const float &           radius_x,
                       const float &           radius_y,
                       const float &           radius_z) -> Array::Pointer;

auto
clip_func(const Device::Pointer & device,
          const Array::Pointer &  src,
          Array::Pointer          dst,
          const float &           min_intensity,
          const float &           max_intensity) -> Array::Pointer;

auto
closing_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 const int &             radius_x,
                 const int &             radius_y,
                 const int &             radius_z) -> Array::Pointer;

auto
closing_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    const float &           radius_x,
                    const float &           radius_y,
                    const float &           radius_z) -> Array::Pointer;

auto
combine_horizontally_func(const Device::Pointer & device,
                          const Array::Pointer &  src0,
                          const Array::Pointer &  src1,
                          Array::Pointer          dst) -> Array::Pointer;

auto
combine_vertically_func(const Device::Pointer & device,
                        const Array::Pointer &  src0,
                        const Array::Pointer &  src1,
                        Array::Pointer          dst) -> Array::Pointer;

auto
concatenate_stacks_func(const Device::Pointer & device,
                        const Array::Pointer &  src0,
                        const Array::Pointer &  src1,
                        Array::Pointer          dst) -> Array::Pointer;

auto
crop_border_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 const int &             border_size) -> Array::Pointer;

// @StRigaud TODO: auto distance_matrix_to_mesh_func;
// @StRigaud TODO: auto flag_existing_labels_func;

auto
degrees_to_radians_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

auto
difference_of_gaussian_func(const Device::Pointer & device,
                            const Array::Pointer &  src,
                            Array::Pointer          dst,
                            const float &           sigma1_x,
                            const float &           sigma1_y,
                            const float &           sigma1_z,
                            const float &           sigma2_x,
                            const float &           sigma2_y,
                            const float &           sigma2_z) -> Array::Pointer;

auto
gamma_correction_func(const Device::Pointer & device, // Tier3
                      const Array::Pointer &  src,
                      Array::Pointer          dst,
                      const float &           gamma) -> Array::Pointer;


// @StRigaud TODO: auto generate_maximum_intensity_between_points_matrix_func;
// @StRigaud TODO: auto generate_mean_intensity_between_points_matrix_func;
// @StRigaud TODO: auto generate_minimum_intensity_between_points_matrix_func;
// @StRigaud TODO: auto generate_standard_deviation_intensity_between_points_matrix_func;
// @StRigaud TODO: auto generate_should_touch_matrix_func;

auto
invert_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

auto
label_spots_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

// @StRigaud TODO: auto large_hessian_eigenvalue_func;

auto
maximum_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

// @StRigaud TODO: auto maximum_of_touching_neighbors_func;
// @StRigaud TODO: auto mean_of_touching_neighbors_func;
// @StRigaud TODO: auto median_of_touching_neighbors_func;

auto
minimum_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

// @StRigaud TODO: auto minimum_of_masked_pixels_func;
// @StRigaud TODO: auto minimum_of_touching_neighbors_func;
// @StRigaud TODO: auto mode_of_touching_neighbors_func;
// @StRigaud TODO: auto neighbors_of_neighbors_func;

auto
opening_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 const int &             radius_x,
                 const int &             radius_y,
                 const int &             radius_z) -> Array::Pointer;

auto
opening_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    const float &           radius_x,
                    const float &           radius_y,
                    const float &           radius_z) -> Array::Pointer;

// @StRigaud TODO: auto pointlist_to_labelled_spots_func;
// @StRigaud TODO: auto prefix_in_x_func;

auto
radians_to_degrees_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

// @StRigaud TODO: auto reduce_stack_func;
// @StRigaud TODO: auto small_hessian_eigenvalue_func;

auto
square_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

auto
standard_deviation_box_func(const Device::Pointer & device,
                            const Array::Pointer &  src,
                            Array::Pointer          dst,
                            const int &             radius_x,
                            const int &             radius_y,
                            const int &             radius_z) -> Array::Pointer;

// @StRigaud TODO: auto standard_deviation_of_touching_neighbors_func;

auto
standard_deviation_sphere_func(const Device::Pointer & device,
                               const Array::Pointer &  src,
                               Array::Pointer          dst,
                               const float &           radius_x,
                               const float &           radius_y,
                               const float &           radius_z) -> Array::Pointer;

auto
sub_stack_func(const Device::Pointer & device,
               const Array::Pointer &  src,
               Array::Pointer          dst,
               const int &             start_z,
               const int &             end_z) -> Array::Pointer;

auto
subtract_images_func(const Device::Pointer & device,
                     const Array::Pointer &  src0,
                     const Array::Pointer &  src1,
                     Array::Pointer          dst) -> Array::Pointer;

auto
sum_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

auto
sum_reduction_x_func(const Device::Pointer & device, // Tier1
                     const Array::Pointer &  src,
                     Array::Pointer          dst,
                     const int &             blocksize) -> Array::Pointer;

// @StRigaud TODO: auto symmetric_maximum_matrix_func;
// @StRigaud TODO: auto symmetric_mean_matrix_func;
// @StRigaud TODO: auto symmetric_minimum_matrix_func;
// @StRigaud TODO: auto symmetric_sum_matrix_func;

auto
top_hat_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 const int &             radius_x,
                 const int &             radius_y,
                 const int &             radius_z) -> Array::Pointer;

auto
top_hat_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    const float &           radius_x,
                    const float &           radius_y,
                    const float &           radius_z) -> Array::Pointer;

// @StRigaud TODO: auto touch_matrix_to_adjacency_matrix_func;
// @StRigaud TODO: auto x_position_of_maximum_x_projection_func;
// @StRigaud TODO: auto x_position_of_minimum_x_projection_func;
// @StRigaud TODO: auto y_position_of_maximum_y_projection_func;
// @StRigaud TODO: auto y_position_of_minimum_y_projection_func;
// @StRigaud TODO: auto z_position_of_maximum_z_projection_func;
// @StRigaud TODO: auto z_position_of_minimum_z_projection_func;
// @StRigaud TODO: auto z_position_projection_func;


} // namespace cle::tier2

#endif // __INCLUDE_TIER2_HPP
