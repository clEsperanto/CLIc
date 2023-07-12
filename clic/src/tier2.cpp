#include "tier2.hpp"
#include "tier0.hpp"
#include "tier1.hpp"


namespace cle::tier2
{

auto
add_images_func(const Device::Pointer & device,
                const Array::Pointer &  src0,
                const Array::Pointer &  src1,
                Array::Pointer          dst) -> Array::Pointer
{
  return tier1::add_images_weighted_func(device, src0, src1, dst, 1, 1);
}

auto
bottom_hat_box_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    const int &             radius_x,
                    const int &             radius_y,
                    const int &             radius_z) -> Array::Pointer
{
  auto temp1 = tier1::maximum_box_func(device, src, nullptr, radius_x, radius_y, radius_z);
  auto temp2 = tier1::minimum_box_func(device, temp1, nullptr, radius_x, radius_y, radius_z);
  return tier1::add_images_weighted_func(device, temp2, src, dst, 1, -1);
}

auto
bottom_hat_sphere_func(const Device::Pointer & device,
                       const Array::Pointer &  src,
                       Array::Pointer          dst,
                       const float &           radius_x,
                       const float &           radius_y,
                       const float &           radius_z) -> Array::Pointer
{
  auto temp1 = tier1::maximum_sphere_func(device, src, nullptr, radius_x, radius_y, radius_z);
  auto temp2 = tier1::minimum_sphere_func(device, temp1, nullptr, radius_x, radius_y, radius_z);
  return tier1::add_images_weighted_func(device, temp2, src, dst, 1, -1);
}

auto
clip_func(const Device::Pointer & device,
          const Array::Pointer &  src,
          Array::Pointer          dst,
          const float &           min_intensity,
          const float &           max_intensity) -> Array::Pointer
{
  auto temp = tier1::maximum_image_and_scalar_func(device, src, nullptr, min_intensity);
  return tier1::minimum_image_and_scalar_func(device, temp, dst, max_intensity);
}

auto
closing_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 const int &             radius_x,
                 const int &             radius_y,
                 const int &             radius_z) -> Array::Pointer
{
  auto temp = tier1::maximum_box_func(device, src, nullptr, radius_x, radius_y, radius_z);
  return tier1::minimum_box_func(device, temp, dst, radius_x, radius_y, radius_z);
}

auto
closing_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    const float &           radius_x,
                    const float &           radius_y,
                    const float &           radius_z) -> Array::Pointer
{
  auto temp = tier1::maximum_sphere_func(device, src, nullptr, radius_x, radius_y, radius_z);
  return tier1::minimum_sphere_func(device, temp, dst, radius_x, radius_y, radius_z);
}

auto
combine_horizontally_func(const Device::Pointer & device,
                          const Array::Pointer &  src0,
                          const Array::Pointer &  src1,
                          Array::Pointer          dst) -> Array::Pointer
{
  return dst; // @StRigaud TODO: implement
}

auto
combine_vertically_func(const Device::Pointer & device,
                        const Array::Pointer &  src0,
                        const Array::Pointer &  src1,
                        Array::Pointer          dst) -> Array::Pointer
{
  return dst; // @StRigaud TODO: implement
}

auto
concatenate_stacks_func(const Device::Pointer & device,
                        const Array::Pointer &  src0,
                        const Array::Pointer &  src1,
                        Array::Pointer          dst) -> Array::Pointer
{
  return dst; // @StRigaud TODO: implement
}

auto
crop_border_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 const int &             border_size) -> Array::Pointer
{
  return dst; // @StRigaud TODO: implement
}

// @StRigaud TODO: auto distance_matrix_to_mesh_func;
// @StRigaud TODO: auto flag_existing_labels_func;

auto
degrees_to_radians_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  return tier1::multiply_image_and_scalar_func(device, src, dst, static_cast<float>(M_PI) / 180.0);
}

auto
difference_of_gaussian_func(const Device::Pointer & device,
                            const Array::Pointer &  src,
                            Array::Pointer          dst,
                            const float &           sigma1_x,
                            const float &           sigma1_y,
                            const float &           sigma1_z,
                            const float &           sigma2_x,
                            const float &           sigma2_y,
                            const float &           sigma2_z) -> Array::Pointer
{
  tier0::create_like(src, dst);
  auto gauss1 = Array::create(dst);
  auto gauss2 = Array::create(dst);
  tier1::gaussian_blur_func(device, src, gauss1, sigma1_x, sigma1_y, sigma1_z);
  tier1::gaussian_blur_func(device, src, gauss2, sigma2_x, sigma2_y, sigma2_z);
  tier1::add_images_weighted_func(device, gauss1, gauss2, dst, 1, -1);
  return dst;
}

auto
gamma_correction_func(const Device::Pointer & device,
                      const Array::Pointer &  src,
                      Array::Pointer          dst,
                      const float &           gamma) -> Array::Pointer
{
  float max_intensity;
  auto  max_int = Array::create(1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  tier2::maximum_of_all_pixels_func(device, src, max_int);
  max_int->read(&max_intensity);

  auto temp1 = tier1::multiply_image_and_scalar_func(device, src, nullptr, 1.0 / max_intensity);
  auto temp2 = tier1::power_func(device, temp1, nullptr, gamma);
  return tier1::multiply_image_and_scalar_func(device, temp2, dst, max_intensity);
}


// @StRigaud TODO: auto generate_maximum_intensity_between_points_matrix_func;
// @StRigaud TODO: auto generate_mean_intensity_between_points_matrix_func;
// @StRigaud TODO: auto generate_minimum_intensity_between_points_matrix_func;
// @StRigaud TODO: auto generate_standard_deviation_intensity_between_points_matrix_func;
// @StRigaud TODO: auto generate_should_touch_matrix_func;

auto
invert_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return tier1::multiply_image_and_scalar_func(device, src, dst, -1);
}

auto
label_spots_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return dst; // @StRigaud TODO: implement
}

// @StRigaud TODO: auto large_hessian_eigenvalue_func;

auto
maximum_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_one(src, dst);
  Array::Pointer tmp = src;
  if (src->depth() > 1)
  {
    auto proj_z = tier1::maximum_z_projection_func(device, tmp, nullptr);
    tmp = proj_z;
  }
  if (src->height() > 1)
  {
    auto proj_y = tier1::maximum_y_projection_func(device, tmp, nullptr);
    tmp = proj_y;
  }
  auto proj_x = tier1::maximum_x_projection_func(device, tmp, nullptr);
  dst = proj_x;
  return dst;
}

// @StRigaud TODO: auto maximum_of_touching_neighbors_func;
// @StRigaud TODO: auto mean_of_touching_neighbors_func;
// @StRigaud TODO: auto median_of_touching_neighbors_func;

auto
minimum_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_one(src, dst);
  Array::Pointer tmp = src;
  if (src->depth() > 1)
  {
    auto proj_z = tier1::minimum_z_projection_func(device, tmp, nullptr);
    tmp = proj_z;
  }
  if (src->height() > 1)
  {
    auto proj_y = tier1::minimum_y_projection_func(device, tmp, nullptr);
    tmp = proj_y;
  }
  auto proj_x = tier1::minimum_x_projection_func(device, tmp, nullptr);
  dst = proj_x;
  return dst;
}

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
                 const int &             radius_z) -> Array::Pointer
{
  auto temp = tier1::minimum_box_func(device, src, nullptr, radius_x, radius_y, radius_z);
  return tier1::maximum_box_func(device, temp, dst, radius_x, radius_y, radius_z);
}

auto
opening_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    const float &           radius_x,
                    const float &           radius_y,
                    const float &           radius_z) -> Array::Pointer
{
  auto temp = tier1::minimum_sphere_func(device, src, nullptr, radius_x, radius_y, radius_z);
  return tier1::maximum_sphere_func(device, temp, dst, radius_x, radius_y, radius_z);
}

// @StRigaud TODO: auto pointlist_to_labelled_spots_func;
// @StRigaud TODO: auto prefix_in_x_func;

auto
radians_to_degrees_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  return tier1::multiply_image_and_scalar_func(device, src, dst, 180.0 / static_cast<float>(M_PI));
}

// @StRigaud TODO: auto reduce_stack_func;
// @StRigaud TODO: auto small_hessian_eigenvalue_func;

auto
square_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return dst;
}

auto
standard_deviation_box_func(const Device::Pointer & device,
                            const Array::Pointer &  src,
                            Array::Pointer          dst,
                            const int &             radius_x,
                            const int &             radius_y,
                            const int &             radius_z) -> Array::Pointer
{
  return tier1::power_func(device, src, dst, 2);
}

// @StRigaud TODO: auto standard_deviation_of_touching_neighbors_func;

auto
standard_deviation_sphere_func(const Device::Pointer & device,
                               const Array::Pointer &  src,
                               Array::Pointer          dst,
                               const float &           radius_x,
                               const float &           radius_y,
                               const float &           radius_z) -> Array::Pointer
{
  return dst; // @StRigaud TODO: implement
}

auto
sub_stack_func(const Device::Pointer & device,
               const Array::Pointer &  src,
               Array::Pointer          dst,
               const int &             start_z,
               const int &             end_z) -> Array::Pointer
{
  return dst; // @StRigaud TODO: implement
}

auto
subtract_images_func(const Device::Pointer & device,
                     const Array::Pointer &  src0,
                     const Array::Pointer &  src1,
                     Array::Pointer          dst) -> Array::Pointer
{
  return tier1::add_images_weighted_func(device, src0, src1, dst, 1, -1);
}

auto
sum_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_one(src, dst);
  Array::Pointer tmp = src;
  if (src->depth() > 1)
  {
    auto proj_z = tier1::sum_z_projection_func(device, tmp, nullptr);
    tmp = proj_z;
  }
  if (src->height() > 1)
  {
    auto proj_y = tier1::sum_y_projection_func(device, tmp, nullptr);
    tmp = proj_y;
  }
  auto proj_x = tier1::sum_x_projection_func(device, tmp, nullptr);
  dst = proj_x;
  return dst;
}

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
                 const int &             radius_z) -> Array::Pointer
{
  auto temp1 = tier1::minimum_box_func(device, src, nullptr, radius_x, radius_y, radius_z);
  auto temp2 = tier1::maximum_box_func(device, temp1, nullptr, radius_x, radius_y, radius_z);
  return tier1::add_images_weighted_func(device, src, temp2, dst, 1, -1);
}

auto
top_hat_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    const float &           radius_x,
                    const float &           radius_y,
                    const float &           radius_z) -> Array::Pointer
{
  auto temp1 = tier1::minimum_sphere_func(device, src, nullptr, radius_x, radius_y, radius_z);
  auto temp2 = tier1::maximum_sphere_func(device, temp1, nullptr, radius_x, radius_y, radius_z);
  return tier1::add_images_weighted_func(device, src, temp2, dst, 1, -1);
}

// @StRigaud TODO: auto touch_matrix_to_adjacency_matrix_func;
// @StRigaud TODO: auto x_position_of_maximum_x_projection_func;
// @StRigaud TODO: auto x_position_of_minimum_x_projection_func;
// @StRigaud TODO: auto y_position_of_maximum_y_projection_func;
// @StRigaud TODO: auto y_position_of_minimum_y_projection_func;
// @StRigaud TODO: auto z_position_of_maximum_z_projection_func;
// @StRigaud TODO: auto z_position_of_minimum_z_projection_func;
// @StRigaud TODO: auto z_position_projection_func;

} // namespace cle::tier2