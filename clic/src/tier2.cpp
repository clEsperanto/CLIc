#include "tier2.hpp"
#include "tier0.hpp"
#include "tier1.hpp"

#include "cle_label_spot_in_x.h"

namespace cle::tier2
{

auto
absolute_difference_func(const Device::Pointer & device,
                         const Array::Pointer &  src0,
                         const Array::Pointer &  src1,
                         Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src0, dst);
  auto tmp = tier1::add_images_weighted_func(device, src0, src1, nullptr, 1, -1);
  return tier1::absolute_func(device, tmp, dst);
}

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
                    int                     radius_x,
                    int                     radius_y,
                    int                     radius_z) -> Array::Pointer
{
  auto temp1 = tier1::maximum_box_func(device, src, nullptr, radius_x, radius_y, radius_z);
  auto temp2 = tier1::minimum_box_func(device, temp1, nullptr, radius_x, radius_y, radius_z);
  return tier1::add_images_weighted_func(device, temp2, src, dst, 1, -1);
}

auto
bottom_hat_sphere_func(const Device::Pointer & device,
                       const Array::Pointer &  src,
                       Array::Pointer          dst,
                       float                   radius_x,
                       float                   radius_y,
                       float                   radius_z) -> Array::Pointer
{
  auto temp1 = tier1::maximum_sphere_func(device, src, nullptr, radius_x, radius_y, radius_z);
  auto temp2 = tier1::minimum_sphere_func(device, temp1, nullptr, radius_x, radius_y, radius_z);
  return tier1::add_images_weighted_func(device, temp2, src, dst, 1, -1);
}

auto
clip_func(const Device::Pointer & device,
          const Array::Pointer &  src,
          Array::Pointer          dst,
          float                   min_intensity,
          float                   max_intensity) -> Array::Pointer
{
  auto temp = tier1::maximum_image_and_scalar_func(device, src, nullptr, min_intensity);
  return tier1::minimum_image_and_scalar_func(device, temp, dst, max_intensity);
}

auto
closing_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 int                     radius_x,
                 int                     radius_y,
                 int                     radius_z) -> Array::Pointer
{
  auto temp = tier1::maximum_box_func(device, src, nullptr, radius_x, radius_y, radius_z);
  return tier1::minimum_box_func(device, temp, dst, radius_x, radius_y, radius_z);
}

auto
closing_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z) -> Array::Pointer
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
  tier0::create_dst(src0, dst, src0->width() + src1->width(), src0->height(), src0->depth(), src0->dtype());
  tier1::paste_func(device, src0, dst, 0, 0, 0);
  tier1::paste_func(device, src1, dst, src0->width(), 0, 0);
  return dst;
}

auto
combine_vertically_func(const Device::Pointer & device,
                        const Array::Pointer &  src0,
                        const Array::Pointer &  src1,
                        Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_dst(src0, dst, src0->width(), src0->height() + src1->height(), src0->depth(), src0->dtype());
  tier1::paste_func(device, src0, dst, 0, 0, 0);
  tier1::paste_func(device, src1, dst, 0, src0->height(), 0);
  return dst;
}

auto
concatenate_stacks_func(const Device::Pointer & device,
                        const Array::Pointer &  src0,
                        const Array::Pointer &  src1,
                        Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_dst(src0, dst, src0->width(), src0->height(), src0->depth() + src1->depth(), src0->dtype());
  tier1::paste_func(device, src0, dst, 0, 0, 0);
  tier1::paste_func(device, src1, dst, 0, 0, src0->depth());
  return dst;
}

// @StRigaud TODO: auto crop_border_func
// @StRigaud TODO: auto distance_matrix_to_mesh_func;

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
                            float                   sigma1_x,
                            float                   sigma1_y,
                            float                   sigma1_z,
                            float                   sigma2_x,
                            float                   sigma2_y,
                            float                   sigma2_z) -> Array::Pointer
{
  tier0::create_like(src, dst);
  auto gauss1 = tier1::gaussian_blur_func(device, src, nullptr, sigma1_x, sigma1_y, sigma1_z);
  auto gauss2 = tier1::gaussian_blur_func(device, src, nullptr, sigma2_x, sigma2_y, sigma2_z);
  return tier1::add_images_weighted_func(device, gauss1, gauss2, dst, 1, -1);
}

auto
extend_labeling_via_voronoi_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  auto flip = Array::create(dst);
  auto flop = Array::create(dst);
  tier1::copy_func(device, src, flip);

  auto flag = Array::create(1, 1, 1, dType::INT32, mType::BUFFER, device);
  flag->fill(0);
  int flag_value = 1;
  int iteration_count = 0;
  while (flag_value > 0)
  {
    if (iteration_count % 2 == 0)
    {
      tier1::onlyzero_overwrite_maximum_box_func(device, flip, flag, flop);
    }
    else
    {
      tier1::onlyzero_overwrite_maximum_box_func(device, flop, flag, flip);
    }
    flag->read(&flag_value);
    flag->fill(0);
    iteration_count++;
  }
  if (iteration_count % 2 == 0)
  {
    flip->copy(dst);
  }
  else
  {
    flop->copy(dst);
  }
  return dst;
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
  tier0::create_like(src, dst);
  dst->fill(0);

  auto spot_count_in_x = tier1::sum_x_projection_func(device, src, nullptr);
  auto spot_count_in_xy = tier1::sum_y_projection_func(device, spot_count_in_x, nullptr);

  const KernelInfo    kernel = { "label_spot_in_x", kernel::label_spot_in_x };
  const ParameterList params = {
    { "src", src }, { "dst", dst }, { "countX", spot_count_in_x }, { "countXY", spot_count_in_xy }
  };
  const RangeArray range = { dst->width(), dst->height(), 1 };
  execute(device, kernel, params, range);
  return dst;
}

// @StRigaud TODO: auto large_hessian_eigenvalue_func;

auto
maximum_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float
{
  Array::Pointer dst = nullptr;
  Array::Pointer tmp = src;
  tier0::create_one(src, dst, dType::FLOAT);
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
  tier1::maximum_x_projection_func(device, tmp, dst);
  float res;
  dst->read(&res);
  return res;
}

// @StRigaud TODO: auto maximum_of_touching_neighbors_func;
// @StRigaud TODO: auto mean_of_touching_neighbors_func;
// @StRigaud TODO: auto median_of_touching_neighbors_func;

auto
minimum_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float
{
  Array::Pointer dst = nullptr;
  Array::Pointer tmp = src;
  tier0::create_one(src, dst, dType::FLOAT);
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
  tier1::minimum_x_projection_func(device, tmp, dst);
  float res;
  dst->read(&res);
  return res;
}

// @StRigaud TODO: auto minimum_of_masked_pixels_func;
// @StRigaud TODO: auto minimum_of_touching_neighbors_func;
// @StRigaud TODO: auto mode_of_touching_neighbors_func;
// @StRigaud TODO: auto neighbors_of_neighbors_func;

auto
opening_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 int                     radius_x,
                 int                     radius_y,
                 int                     radius_z) -> Array::Pointer
{
  auto temp = tier1::minimum_box_func(device, src, nullptr, radius_x, radius_y, radius_z);
  return tier1::maximum_box_func(device, temp, dst, radius_x, radius_y, radius_z);
}

auto
opening_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z) -> Array::Pointer
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
squared_difference_func(const Device::Pointer & device,
                        const Array::Pointer &  src0,
                        const Array::Pointer &  src1,
                        Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src0, dst);
  auto tmp = tier1::add_images_weighted_func(device, src0, src1, nullptr, 1, -1);
  return tier1::power_func(device, tmp, dst, 2);
}

// @StRigaud TODO: auto standard_deviation_box_func
// @StRigaud TODO: auto standard_deviation_of_touching_neighbors_func
// @StRigaud TODO: auto standard_deviation_sphere_func
// @StRigaud TODO: auto sub_stack_func

auto
subtract_images_func(const Device::Pointer & device,
                     const Array::Pointer &  src0,
                     const Array::Pointer &  src1,
                     Array::Pointer          dst) -> Array::Pointer
{
  return tier1::add_images_weighted_func(device, src0, src1, dst, 1, -1);
}

auto
sum_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float
{
  Array::Pointer dst = nullptr;
  Array::Pointer tmp = src;
  tier0::create_one(src, dst, dType::FLOAT);
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
  tier1::sum_x_projection_func(device, tmp, dst);
  float res;
  dst->read(&res);
  return res;
}

// @StRigaud TODO: auto symmetric_maximum_matrix_func;
// @StRigaud TODO: auto symmetric_mean_matrix_func;
// @StRigaud TODO: auto symmetric_minimum_matrix_func;
// @StRigaud TODO: auto symmetric_sum_matrix_func;

auto
top_hat_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 int                     radius_x,
                 int                     radius_y,
                 int                     radius_z) -> Array::Pointer
{
  auto temp1 = tier1::minimum_box_func(device, src, nullptr, radius_x, radius_y, radius_z);
  auto temp2 = tier1::maximum_box_func(device, temp1, nullptr, radius_x, radius_y, radius_z);
  return tier1::add_images_weighted_func(device, src, temp2, dst, 1, -1);
}

auto
top_hat_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z) -> Array::Pointer
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