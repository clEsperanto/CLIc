#include "tier1.hpp"
#include "utils.hpp"

#include "cle_absolute.h"
#include "cle_add_image_and_scalar.h"
#include "cle_add_images_weighted.h"
// #include "cle_average_distance_of_n_far_off_distances.h"
// #include "cle_average_distance_of_n_far_off_distances.h"
// #include "cle_average_distance_of_n_shortest_distances.h"
// #include "cle_average_distance_of_n_shortest_distances.h"
// #include "cle_average_distance_of_n_nearest_distances.h"
// #include "cle_average_distance_of_touching_neighbors.h"
#include "cle_binary_and.h"
#include "cle_binary_edge_detection.h"
#include "cle_binary_not.h"
#include "cle_binary_or.h"
#include "cle_binary_subtract.h"
#include "cle_binary_xor.h"
#include "cle_block_enumerate.h"
#include "cle_convolve.h"
#include "cle_copy.h"
#include "cle_copy_horizontal_slice_from.h"
#include "cle_copy_horizontal_slice_to.h"
#include "cle_copy_slice_from.h"
#include "cle_copy_slice_to.h"
#include "cle_copy_vertical_slice_from.h"
#include "cle_copy_vertical_slice_to.h"
#include "cle_crop.h"
#include "cle_cubic_root.h"
#include "cle_detect_label_edges.h"
#include "cle_dilate_box.h"
// #include "cle_dilate_box_slice_by_slice.h"
#include "cle_dilate_sphere.h"
// #include "cle_dilate_sphere_slice_by_slice.h"
#include "cle_divide_images.h"
#include "cle_divide_scalar_by_image.h"
// #include "cle_draw_box.h"
// #include "cle_draw_line.h"
// #include "cle_draw_sphere.h"
// #include "cle_downsample_slice_by_slice_half_median.h"
#include "cle_equal.h"
#include "cle_equal_constant.h"
#include "cle_erode_box.h"
// #include "cle_erode_box_slice_by_slice.h"
#include "cle_erode_sphere.h"
// #include "cle_erode_sphere_slice_by_slice.h"
#include "cle_exponential.h"
#include "cle_flip.h"
#include "cle_gaussian_blur_separable.h"
// #include "cle_generate_angle_matrix.h"
// #include "cle_generate_binary_overlap_matrix.h"
#include "cle_generate_distance_matrix.h"
#include "cle_gradient_x.h"
#include "cle_gradient_y.h"
#include "cle_gradient_z.h"
#include "cle_greater.h"
#include "cle_greater_constant.h"
#include "cle_greater_or_equal.h"
#include "cle_greater_or_equal_constant.h"
#include "cle_hessian_eigenvalues.h"
#include "cle_laplace_box.h"
#include "cle_laplace_diamond.h"
#include "cle_local_cross_correlation.h"
#include "cle_logarithm.h"
#include "cle_mask.h"
#include "cle_mask_label.h"
#include "cle_maximum_image_and_scalar.h"
#include "cle_maximum_images.h"
#include "cle_maximum_separable.h"
// #include "cle_maximum_distance_of_touching_neighbors.h"
// #include "cle_maximum_distance_of_n_shortest_distances.h"
// #include "cle_maximum_distance_of_n_shortest_distances.h"
#include "cle_maximum_x_projection.h"
#include "cle_maximum_y_projection.h"
#include "cle_maximum_z_projection.h"
#include "cle_mean_separable.h"
#include "cle_mean_sphere.h"
#include "cle_mean_x_projection.h"
#include "cle_mean_y_projection.h"
#include "cle_mean_z_projection.h"
#include "cle_median_box.h"
#include "cle_median_sphere.h"
#include "cle_minimum_separable.h"
// #include "cle_minimum_distance_of_touching_neighbors.h"
#include "cle_minimum_image_and_scalar.h"
#include "cle_minimum_images.h"
#include "cle_minimum_of_masked_pixels_reduction.h"
#include "cle_minimum_x_projection.h"
#include "cle_minimum_y_projection.h"
#include "cle_minimum_z_projection.h"
#include "cle_mode_box.h"
#include "cle_mode_sphere.h"
#include "cle_modulo_images.h"
#include "cle_multiply_image_and_position.h"
#include "cle_multiply_image_and_scalar.h"
#include "cle_multiply_images.h"
// #include "cle_n_closest_points.h"
#include "cle_nan_to_num.h"
#include "cle_nonzero_maximum_box.h"
#include "cle_nonzero_maximum_diamond.h"
#include "cle_nonzero_minimum_box.h"
#include "cle_nonzero_minimum_diamond.h"
#include "cle_not_equal.h"
#include "cle_not_equal_constant.h"
#include "cle_onlyzero_overwrite_maximum_box.h"
#include "cle_onlyzero_overwrite_maximum_diamond.h"
#include "cle_paste.h"
// #include "cle_point_index_list_to_mesh.h"
// #include "cle_point_index_list_to_touch_matrix.h"
#include "cle_power.h"
#include "cle_power_images.h"
#include "cle_range.h"
// #include "cle_read_values_from_map.h"
#include "cle_read_values_from_positions.h"
#include "cle_replace_value.h"
#include "cle_replace_values.h"
// #include "cle_resample.h"
// #include "cle_touch_matrix_to_mesh.h"
#include "cle_maximum_sphere.h"
#include "cle_minimum_sphere.h"
#include "cle_multiply_matrix.h"
#include "cle_reciprocal.h"
#include "cle_set.h"
#include "cle_set_column.h"
#include "cle_set_image_borders.h"
#include "cle_set_nonzero_pixels_to_pixelindex.h"
#include "cle_set_plane.h"
#include "cle_set_ramp_x.h"
#include "cle_set_ramp_y.h"
#include "cle_set_ramp_z.h"
#include "cle_set_row.h"
#include "cle_set_where_x_equals_y.h"
#include "cle_set_where_x_greater_than_y.h"
#include "cle_set_where_x_smaller_than_y.h"
#include "cle_sign.h"
#include "cle_smaller.h"
#include "cle_smaller_constant.h"
#include "cle_smaller_or_equal.h"
#include "cle_smaller_or_equal_constant.h"
#include "cle_sobel.h"
#include "cle_square_root.h"
#include "cle_std_z_projection.h"
// #include "cle_inferior_superior.h"
#include "cle_subtract_image_from_scalar.h"
#include "cle_sum_reduction_x.h"
#include "cle_sum_x_projection.h"
#include "cle_sum_y_projection.h"
#include "cle_sum_z_projection.h"
// #include "cle_superior_inferior.h"
#include "cle_transpose_xy.h"
#include "cle_transpose_xz.h"
#include "cle_transpose_yz.h"
#include "cle_undefined_to_zero.h"
#include "cle_variance_box.h"
#include "cle_variance_sphere.h"
#include "cle_write_values_to_positions.h"
#include "cle_x_position_of_maximum_x_projection.h"
#include "cle_x_position_of_minimum_x_projection.h"
#include "cle_y_position_of_maximum_y_projection.h"
#include "cle_y_position_of_minimum_y_projection.h"
#include "cle_z_position_of_maximum_z_projection.h"
#include "cle_z_position_of_minimum_z_projection.h"


namespace cle::tier1
{

auto
absolute_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "absolute", kernel::absolute };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  std::cout << "Hello A" << std::endl;
  execute(device, kernel, params, range);
  std::cout << "Hello Z" << std::endl;
  return dst;
}

auto
add_images_weighted_func(const Device::Pointer & device,
                         const Array::Pointer &  src0,
                         const Array::Pointer &  src1,
                         Array::Pointer          dst,
                         float                   factor0,
                         float                   factor1) -> Array::Pointer
{
  tier0::create_like(src0, dst, dType::FLOAT);
  const KernelInfo    kernel = { "add_images_weighted", kernel::add_images_weighted };
  const ParameterList params = {
    { "src0", src0 }, { "src1", src1 }, { "dst", dst }, { "scalar0", factor0 }, { "scalar1", factor1 }
  };
  const RangeArray range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
add_image_and_scalar_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "add_image_and_scalar", kernel::add_image_and_scalar };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar", scalar } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

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
                Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src0, dst, dType::UINT8);
  const KernelInfo    kernel = { "binary_and", kernel::binary_and };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
binary_edge_detection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::UINT8);
  const KernelInfo    kernel = { "binary_edge_detection", kernel::binary_edge_detection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
binary_not_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::UINT8);
  const KernelInfo    kernel = { "binary_not", kernel::binary_not };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
binary_or_func(const Device::Pointer & device,
               const Array::Pointer &  src0,
               const Array::Pointer &  src1,
               Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src0, dst, dType::UINT8);
  const KernelInfo    kernel = { "binary_or", kernel::binary_or };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
binary_subtract_func(const Device::Pointer & device,
                     const Array::Pointer &  src0,
                     const Array::Pointer &  src1,
                     Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src0, dst, dType::UINT8);
  const KernelInfo    kernel = { "binary_subtract", kernel::binary_subtract };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
binary_xor_func(const Device::Pointer & device,
                const Array::Pointer &  src0,
                const Array::Pointer &  src1,
                Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src0, dst, dType::UINT8);
  const KernelInfo    kernel = { "binary_xor", kernel::binary_xor };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
block_enumerate_func(const Device::Pointer & device,
                     const Array::Pointer &  src0,
                     const Array::Pointer &  src1,
                     Array::Pointer          dst,
                     int                     blocksize) -> Array::Pointer
{
  tier0::create_like(src0, dst);
  const KernelInfo    kernel = { "block_enumerate", kernel::block_enumerate };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst }, { "index", blocksize } };
  const RangeArray    range = { src1->width(), src1->height(), src1->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
convolve_func(const Device::Pointer & device,
              const Array::Pointer &  src0,
              const Array::Pointer &  src1,
              Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src0, dst);
  const KernelInfo    kernel = { "convolve", kernel::convolve };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
copy_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "copy", kernel::copy };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
copy_slice_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int slice)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  const ParameterList params = { { "src", src }, { "dst", dst }, { "index", slice } };
  KernelInfo          kernel;
  RangeArray          range;
  if (dst->depth() > 1)
  {
    kernel = { "copy_slice_to", kernel::copy_slice_to };
    range = { src->width(), src->height(), 1 };
  }
  else
  {
    kernel = { "copy_slice_from", kernel::copy_slice_from };
    range = { dst->width(), dst->height(), dst->depth() };
  }
  execute(device, kernel, params, range);
  return dst;
}

auto
copy_horizontal_slice_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int slice)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  const ParameterList params = { { "src", src }, { "dst", dst }, { "index", slice } };
  KernelInfo          kernel;
  RangeArray          range;
  if (dst->depth() > 1)
  {
    kernel = { "copy_horizontal_slice_to", kernel::copy_horizontal_slice_to };
    range = { dst->width(), dst->height(), dst->depth() };
  }
  else
  {
    kernel = { "copy_horizontal_slice_from", kernel::copy_horizontal_slice_from };
    range = { dst->width(), dst->height(), dst->depth() };
  }
  execute(device, kernel, params, range);
  return dst;
}

auto
copy_vertical_slice_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int slice)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  const ParameterList params = { { "src", src }, { "dst", dst }, { "index", slice } };
  KernelInfo          kernel;
  RangeArray          range;
  if (dst->depth() > 1)
  {
    kernel = { "copy_vertical_slice_to", kernel::copy_vertical_slice_to };
    range = { src->width(), src->height(), 1 };
  }
  else
  {
    kernel = { "copy_vertical_slice_from", kernel::copy_vertical_slice_from };
    range = { dst->width(), dst->height(), dst->depth() };
  }
  execute(device, kernel, params, range);
  return dst;
}

// count_touching_neighbors_func

auto
crop_func(const Device::Pointer & device,
          const Array::Pointer &  src,
          Array::Pointer          dst,
          int                     start_x,
          int                     start_y,
          int                     start_z,
          int                     width,
          int                     height,
          int                     depth) -> Array::Pointer
{
  tier0::create_dst(src, dst, width, height, depth);
  const KernelInfo    kernel = { "crop", kernel::crop };
  const ParameterList params = {
    { "src", src }, { "dst", dst }, { "index0", start_x }, { "index1", start_y }, { "index2", start_z }
  };
  const RangeArray range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
cubic_root_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "cubic_root", kernel::cubic_root };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
detect_label_edges_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::UINT8);
  const KernelInfo    kernel = { "detect_label_edges", kernel::detect_label_edges };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
dilate_box_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "dilate_box", kernel::dilate_box };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
dilate_sphere_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "dilate_sphere", kernel::dilate_sphere };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

// dilate_sphere_slice_by_slice_func

auto
divide_images_func(const Device::Pointer & device,
                   const Array::Pointer &  src0,
                   const Array::Pointer &  src1,
                   Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src0, dst);
  const KernelInfo    kernel = { "divide_images", kernel::divide_images };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
divide_scalar_by_image_func(const Device::Pointer & device,
                            const Array::Pointer &  src,
                            Array::Pointer          dst,
                            float                   scalar) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "divide_scalar_by_image", kernel::divide_scalar_by_image };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar", scalar } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

// draw_box_func
// draw_sphere_func
// draw_line_func
// downsample_slice_by_slice_half_median_func

auto
equal_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_like(src0, dst, dType::UINT8);
  const KernelInfo    kernel = { "equal", kernel::equal };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
equal_constant_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::UINT8);
  const KernelInfo    kernel = { "equal_constant", kernel::equal_constant };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar", scalar } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
erode_box_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "erode_box", kernel::erode_box };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

// erode_box_slice_by_slice_func

auto
erode_sphere_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "erode_sphere", kernel::erode_sphere };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

// erode_sphere_slice_by_slice_func

auto
exponential_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "exponential", kernel::exponential };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
flip_func(const Device::Pointer & device,
          const Array::Pointer &  src,
          Array::Pointer          dst,
          bool                    flip_x,
          bool                    flip_y,
          bool                    flip_z) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "flip", kernel::flip };
  const ParameterList params = {
    { "src", src }, { "dst", dst }, { "index0", int(flip_x) }, { "index1", int(flip_y) }, { "index2", int(flip_z) }
  };
  const RangeArray range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
gaussian_blur_func(const Device::Pointer & device,
                   const Array::Pointer &  src,
                   Array::Pointer          dst,
                   float                   sigma_x,
                   float                   sigma_y,
                   float                   sigma_z) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  Array::Pointer temp = src;
  if (temp->dtype() != dType::FLOAT)
  {
    temp = Array::create(dst);
    tier1::copy_func(device, src, temp);
  }
  const KernelInfo kernel = { "gaussian_blur_separable", kernel::gaussian_blur_separable };
  tier0::execute_separable_func(device,
                                kernel,
                                temp,
                                dst,
                                { sigma_x, sigma_y, sigma_z },
                                { sigma2kernelsize(sigma_x), sigma2kernelsize(sigma_y), sigma2kernelsize(sigma_z) });
  return dst;
}

// generate_angle_matrix_func
// generate_binary_overlap_matrix_func

auto
generate_distance_matrix_func(const Device::Pointer & device,
                              const Array::Pointer &  src0,
                              const Array::Pointer &  src1,
                              Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_dst(src0, dst, src0->width() + 1, src0->width() + 1, 1, dType::FLOAT);
  dst->fill(0);
  const KernelInfo    kernel = { "generate_distance_matrix", kernel::generate_distance_matrix };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
gradient_x_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "gradient_x", kernel::gradient_x };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
gradient_y_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "gradient_y", kernel::gradient_y };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
gradient_z_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "gradient_z", kernel::gradient_z };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
greater_func(const Device::Pointer & device,
             const Array::Pointer &  src0,
             const Array::Pointer &  src1,
             Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src0, dst, dType::UINT8);
  const KernelInfo    kernel = { "greater", kernel::greater };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
greater_constant_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::UINT8);
  const KernelInfo    kernel = { "greater_constant", kernel::greater_constant };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar", scalar } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
greater_or_equal_func(const Device::Pointer & device,
                      const Array::Pointer &  src0,
                      const Array::Pointer &  src1,
                      Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src0, dst, dType::UINT8);
  const KernelInfo    kernel = { "greater_or_equal", kernel::greater_or_equal };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
greater_or_equal_constant_func(const Device::Pointer & device,
                               const Array::Pointer &  src,
                               Array::Pointer          dst,
                               float                   scalar) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::UINT8);
  const KernelInfo    kernel = { "greater_or_equal_constant", kernel::greater_or_equal_constant };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar", scalar } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
hessian_eigenvalues_func(const Device::Pointer & device,
                         const Array::Pointer &  src,
                         Array::Pointer          small_eigenvalue,
                         Array::Pointer          middle_eigenvalue,
                         Array::Pointer          large_eigenvalue) -> std::vector<Array::Pointer>
{
  // TODO: check when src is 1D
  tier0::create_like(src, small_eigenvalue, dType::FLOAT);
  tier0::create_like(src, large_eigenvalue, dType::FLOAT);
  if (src->depth() > 1)
  {
    tier0::create_like(src, middle_eigenvalue, dType::FLOAT);
  }
  else
  {
    // no middle eigenvalue for 2D images, we replace the image by a scalar to save memory
    tier0::create_one(src, middle_eigenvalue, dType::FLOAT);
  }
  const KernelInfo    kernel = { "hessian_eigenvalues", kernel::hessian_eigenvalues };
  const ParameterList params = { { "src", src },
                                 { "small_eigenvalue", small_eigenvalue },
                                 { "middle_eigenvalue", middle_eigenvalue },
                                 { "large_eigenvalue", large_eigenvalue } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  if (src->depth() == 1)
  {
    return { small_eigenvalue, large_eigenvalue };
  }
  return { small_eigenvalue, middle_eigenvalue, large_eigenvalue };
}

// auto
// inferior_superior(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
// {
//   tier0::create_like(src, dst, dType::UINT8);
//   if (src->dtype() != dType::UINT8)
//   {
//     throw std::runtime_error("inferior_superior only supports UINT8 images");
//   }
//   const KernelInfo    kernel = { "inferior_superior", kernel::inferior_superior };
//   const ParameterList params = { { "src", src }, { "dst", dst } };
//   const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
//   execute(device, kernel, params, range);
//   return dst;
// }

auto
laplace_box_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "laplace_box", kernel::laplace_box };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
laplace_diamond_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "laplace_diamond", kernel::laplace_diamond };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
local_cross_correlation_func(const Device::Pointer & device,
                             const Array::Pointer &  src0,
                             const Array::Pointer &  src1,
                             Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src0, dst);
  const KernelInfo    kernel = { "local_cross_correlation", kernel::local_cross_correlation };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
logarithm_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "logarithm", kernel::logarithm };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
mask_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & mask, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "mask", kernel::mask };
  const ParameterList params = { { "src0", src }, { "src1", mask }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
mask_label_func(const Device::Pointer & device,
                const Array::Pointer &  src0,
                const Array::Pointer &  src1,
                Array::Pointer          dst,
                float                   label) -> Array::Pointer
{
  tier0::create_like(src0, dst);
  const KernelInfo    kernel = { "mask_label", kernel::mask_label };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst }, { "scalar", label } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
maximum_image_and_scalar_func(const Device::Pointer & device,
                              const Array::Pointer &  src,
                              Array::Pointer          dst,
                              float                   scalar) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "maximum_image_and_scalar", kernel::maximum_image_and_scalar };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar", scalar } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
maximum_images_func(const Device::Pointer & device,
                    const Array::Pointer &  src0,
                    const Array::Pointer &  src1,
                    Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src0, dst);
  const KernelInfo    kernel = { "maximum_images", kernel::maximum_images };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
maximum_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 int                     radius_x,
                 int                     radius_y,
                 int                     radius_z) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo kernel = { "maximum_separable", kernel::maximum_separable };
  tier0::execute_separable_func(
    device,
    kernel,
    src,
    dst,
    { static_cast<float>(radius_x), static_cast<float>(radius_y), static_cast<float>(radius_z) },
    { radius2kernelsize(radius_x), radius2kernelsize(radius_y), radius2kernelsize(radius_z) });
  return dst;
}

// maximum_distance_of_touching_neighbors_func
// maximum_distance_of_n_shortest_distances_func

auto
maximum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_zy(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "maximum_x_projection", kernel::maximum_x_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
maximum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_xz(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "maximum_y_projection", kernel::maximum_y_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
maximum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_xy(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "maximum_z_projection", kernel::maximum_z_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
mean_box_func(const Device::Pointer & device,
              const Array::Pointer &  src,
              Array::Pointer          dst,
              int                     radius_x,
              int                     radius_y,
              int                     radius_z) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo kernel = { "mean_separable", kernel::mean_separable };
  tier0::execute_separable_func(
    device,
    kernel,
    src,
    dst,
    { static_cast<float>(radius_x), static_cast<float>(radius_y), static_cast<float>(radius_z) },
    { radius2kernelsize(radius_x), radius2kernelsize(radius_y), radius2kernelsize(radius_z) });
  return dst;
}

auto
mean_sphere_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 int                     radius_x,
                 int                     radius_y,
                 int                     radius_z) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "mean_sphere", kernel::mean_sphere };
  const ParameterList params = { { "src", src },
                                 { "dst", dst },
                                 { "scalar0", radius2kernelsize(radius_x) },
                                 { "scalar1", radius2kernelsize(radius_y) },
                                 { "scalar2", radius2kernelsize(radius_z) } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
mean_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_zy(src, dst);
  const KernelInfo    kernel = { "mean_x_projection", kernel::mean_x_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
mean_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xz(src, dst);
  const KernelInfo    kernel = { "mean_y_projection", kernel::mean_y_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
mean_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xy(src, dst);
  const KernelInfo    kernel = { "mean_z_projection", kernel::mean_z_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
median_box_func(const Device::Pointer & device,
                const Array::Pointer &  src,
                Array::Pointer          dst,
                int                     radius_x,
                int                     radius_y,
                int                     radius_z) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "median_box", kernel::median_box };
  const ParameterList params = { { "src", src },
                                 { "dst", dst },
                                 { "scalar0", radius2kernelsize(radius_x) },
                                 { "scalar1", radius2kernelsize(radius_y) },
                                 { "scalar2", radius2kernelsize(radius_z) } };
  const ConstantList  constants = {
    { "MAX_ARRAY_SIZE", radius2kernelsize(radius_x) * radius2kernelsize(radius_y) * radius2kernelsize(radius_z) }
  };
  const RangeArray range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range, constants);
  return dst;
}

auto
median_sphere_func(const Device::Pointer & device,
                   const Array::Pointer &  src,
                   Array::Pointer          dst,
                   int                     radius_x,
                   int                     radius_y,
                   int                     radius_z) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "median_sphere", kernel::median_sphere };
  const ParameterList params = { { "src", src },
                                 { "dst", dst },
                                 { "scalar0", radius2kernelsize(radius_x) },
                                 { "scalar1", radius2kernelsize(radius_y) },
                                 { "scalar2", radius2kernelsize(radius_z) } };
  const ConstantList  constants = {
    { "MAX_ARRAY_SIZE", radius2kernelsize(radius_x) * radius2kernelsize(radius_y) * radius2kernelsize(radius_z) }
  };
  const RangeArray range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range, constants);
  return dst;
}

auto
minimum_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 int                     radius_x,
                 int                     radius_y,
                 int                     radius_z) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo kernel = { "minimum_separable", kernel::minimum_separable };
  tier0::execute_separable_func(
    device,
    kernel,
    src,
    dst,
    { static_cast<float>(radius_x), static_cast<float>(radius_y), static_cast<float>(radius_z) },
    { radius2kernelsize(radius_x), radius2kernelsize(radius_y), radius2kernelsize(radius_z) });
  return dst;
}

// minimum_distance_of_touching_neighbors_func

auto
minimum_image_and_scalar_func(const Device::Pointer & device,
                              const Array::Pointer &  src,
                              Array::Pointer          dst,
                              float                   scalar) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "minimum_image_and_scalar", kernel::minimum_image_and_scalar };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar", scalar } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
minimum_images_func(const Device::Pointer & device,
                    const Array::Pointer &  src0,
                    const Array::Pointer &  src1,
                    Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src0, dst);
  const KernelInfo    kernel = { "minimum_images", kernel::minimum_images };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
minimum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_zy(src, dst);
  const KernelInfo    kernel = { "minimum_x_projection", kernel::minimum_x_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
minimum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_xz(src, dst);
  const KernelInfo    kernel = { "minimum_y_projection", kernel::minimum_y_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
minimum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_xy(src, dst);
  const KernelInfo    kernel = { "minimum_z_projection", kernel::minimum_z_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
minimum_of_masked_pixels_reduction_func(const Device::Pointer & device,
                                        const Array::Pointer &  src,
                                        const Array::Pointer &  mask,
                                        Array::Pointer          reduced_src,
                                        Array::Pointer          reduced_mask) -> Array::Pointer
{
  tier0::create_xy(src, reduced_src);
  tier0::create_xy(mask, reduced_mask);
  const KernelInfo    kernel = { "minimum_of_masked_pixels_reduction", kernel::minimum_of_masked_pixels_reduction };
  const ParameterList params = {
    { "src", src }, { "mask", mask }, { "dst_src", reduced_src }, { "dst_mask", reduced_mask }
  };
  const RangeArray range = { reduced_src->width(), reduced_src->height(), reduced_src->depth() };
  execute(device, kernel, params, range);
  return reduced_src;
}


auto
mode_box_func(const Device::Pointer & device,
              const Array::Pointer &  src,
              Array::Pointer          dst,
              int                     radius_x,
              int                     radius_y,
              int                     radius_z) -> Array::Pointer
{
  if (src->dtype() != dType::UINT8)
  {
    std::cerr << "Warning: mode only support uint8 pixel type." << std::endl;
  }
  tier0::create_like(src, dst, dType::UINT8);
  const KernelInfo    kernel = { "mode_box", kernel::mode_box };
  const ParameterList params = { { "src", src },
                                 { "dst", dst },
                                 { "scalar0", radius2kernelsize(radius_x) },
                                 { "scalar1", radius2kernelsize(radius_y) },
                                 { "scalar2", radius2kernelsize(radius_z) } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
mode_sphere_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 int                     radius_x,
                 int                     radius_y,
                 int                     radius_z) -> Array::Pointer
{
  if (src->dtype() != dType::UINT8)
  {
    std::cerr << "Warning: mode only support uint8 pixel type." << std::endl;
  }
  tier0::create_like(src, dst, dType::UINT8);
  const KernelInfo    kernel = { "mode_sphere", kernel::mode_sphere };
  const ParameterList params = { { "src", src },
                                 { "dst", dst },
                                 { "scalar0", radius2kernelsize(radius_x) },
                                 { "scalar1", radius2kernelsize(radius_y) },
                                 { "scalar2", radius2kernelsize(radius_z) } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
modulo_images_func(const Device::Pointer & device,
                   const Array::Pointer &  src0,
                   const Array::Pointer &  src1,
                   Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src0, dst);
  const KernelInfo    kernel = { "modulo_images", kernel::modulo_images };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
multiply_image_and_position_func(const Device::Pointer & device,
                                 const Array::Pointer &  src,
                                 Array::Pointer          dst,
                                 int                     dimension) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "multiply_image_and_position", kernel::multiply_image_and_position };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "index", dimension } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
multiply_image_and_scalar_func(const Device::Pointer & device,
                               const Array::Pointer &  src,
                               Array::Pointer          dst,
                               float                   scalar) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "multiply_image_and_scalar", kernel::multiply_image_and_scalar };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar", scalar } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
multiply_images_func(const Device::Pointer & device,
                     const Array::Pointer &  src0,
                     const Array::Pointer &  src1,
                     Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src0, dst);
  const KernelInfo    kernel = { "multiply_images", kernel::multiply_images };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

// n_closest_points_func

auto
nan_to_num_func(const Device::Pointer & device,
                const Array::Pointer &  src,
                Array::Pointer          dst,
                float                   nan,
                float                   posinf,
                float                   neginf) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "nan_to_num", kernel::nan_to_num };
  const ParameterList params = {
    { "src", src }, { "dst", dst }, { "nan", nan }, { "pinf", posinf }, { "ninf", neginf }
  };
  const RangeArray range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
nonzero_maximum_box_func(const Device::Pointer & device,
                         const Array::Pointer &  src,
                         Array::Pointer          dst0,
                         Array::Pointer          dst1) -> Array::Pointer
{
  tier0::create_like(src, dst1);
  dst1->fill(0);
  const KernelInfo    kernel = { "nonzero_maximum_box", kernel::nonzero_maximum_box };
  const ParameterList params = { { "src", src }, { "dst0", dst0 }, { "dst1", dst1 } };
  const RangeArray    range = { dst1->width(), dst1->height(), dst1->depth() };
  execute(device, kernel, params, range);
  return dst1;
}

auto
nonzero_maximum_diamond_func(const Device::Pointer & device,
                             const Array::Pointer &  src,
                             Array::Pointer          dst0,
                             Array::Pointer          dst1) -> Array::Pointer
{
  tier0::create_like(src, dst1);
  dst1->fill(0);
  const KernelInfo    kernel = { "nonzero_maximum_diamond", kernel::nonzero_maximum_diamond };
  const ParameterList params = { { "src", src }, { "dst0", dst0 }, { "dst1", dst1 } };
  const RangeArray    range = { dst1->width(), dst1->height(), dst1->depth() };
  execute(device, kernel, params, range);
  return dst1;
}

auto
nonzero_minimum_box_func(const Device::Pointer & device,
                         const Array::Pointer &  src,
                         Array::Pointer          dst0,
                         Array::Pointer          dst1) -> Array::Pointer
{
  tier0::create_like(src, dst1);
  dst1->fill(0);
  const KernelInfo    kernel = { "nonzero_minimum_box", kernel::nonzero_minimum_box };
  const ParameterList params = { { "src", src }, { "dst0", dst0 }, { "dst1", dst1 } };
  const RangeArray    range = { dst1->width(), dst1->height(), dst1->depth() };
  execute(device, kernel, params, range);
  return dst1;
}

auto
nonzero_minimum_diamond_func(const Device::Pointer & device,
                             const Array::Pointer &  src,
                             Array::Pointer          dst0,
                             Array::Pointer          dst1) -> Array::Pointer
{
  tier0::create_like(src, dst1);
  dst1->fill(0);
  const KernelInfo    kernel = { "nonzero_minimum_diamond", kernel::nonzero_minimum_diamond };
  const ParameterList params = { { "src", src }, { "dst0", dst0 }, { "dst1", dst1 } };
  const RangeArray    range = { dst1->width(), dst1->height(), dst1->depth() };
  execute(device, kernel, params, range);
  return dst1;
}

auto
not_equal_func(const Device::Pointer & device,
               const Array::Pointer &  src0,
               const Array::Pointer &  src1,
               Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src0, dst, dType::UINT8);
  const KernelInfo    kernel = { "not_equal", kernel::not_equal };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
not_equal_constant_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::UINT8);
  const KernelInfo    kernel = { "not_equal_constant", kernel::not_equal_constant };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar", scalar } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
paste_func(const Device::Pointer & device,
           const Array::Pointer &  src,
           Array::Pointer          dst,
           int                     index_x,
           int                     index_y,
           int                     index_z) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "paste", kernel::paste };
  const ParameterList params = {
    { "src", src }, { "dst", dst }, { "scalar0", index_x }, { "scalar1", index_y }, { "scalar2", index_z }
  };
  const RangeArray range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
onlyzero_overwrite_maximum_box_func(const Device::Pointer & device,
                                    const Array::Pointer &  src,
                                    Array::Pointer          dst0,
                                    Array::Pointer          dst1) -> Array::Pointer
{
  tier0::create_like(src, dst1);
  const KernelInfo    kernel = { "onlyzero_overwrite_maximum_box", kernel::onlyzero_overwrite_maximum_box };
  const ParameterList params = { { "src", src }, { "dst0", dst0 }, { "dst1", dst1 } };
  const RangeArray    range = { dst1->width(), dst1->height(), dst1->depth() };
  execute(device, kernel, params, range);
  return dst1;
}

auto
onlyzero_overwrite_maximum_diamond_func(const Device::Pointer & device,
                                        const Array::Pointer &  src,
                                        Array::Pointer          dst0,
                                        Array::Pointer          dst1) -> Array::Pointer
{
  tier0::create_like(src, dst1);
  const KernelInfo    kernel = { "onlyzero_overwrite_maximum_diamond", kernel::onlyzero_overwrite_maximum_diamond };
  const ParameterList params = { { "src", src }, { "dst0", dst0 }, { "dst1", dst1 } };
  const RangeArray    range = { dst1->width(), dst1->height(), dst1->depth() };
  execute(device, kernel, params, range);
  return dst1;
}

// point_index_list_to_mesh_func
// point_index_list_to_touch_matrix_func

auto
power_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "power", kernel::power };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar", scalar } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
power_images_func(const Device::Pointer & device,
                  const Array::Pointer &  src0,
                  const Array::Pointer &  src1,
                  Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src0, dst);
  const KernelInfo    kernel = { "power_images", kernel::power_images };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

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
           int                     step_z) -> Array::Pointer
{
  tier0::create_dst(src,
                    dst,
                    abs(stop_x - start_x) / std::max(std::abs(step_x), 1),
                    abs(stop_y - start_y) / std::max(std::abs(step_y), 1),
                    abs(stop_z - start_z) / std::max(std::abs(step_z), 1),
                    src->dtype());

  correct_range(&start_x, &stop_x, &step_x, static_cast<int>(src->width()));
  correct_range(&start_y, &stop_y, &step_y, static_cast<int>(src->height()));
  correct_range(&start_z, &stop_z, &step_z, static_cast<int>(src->depth()));

  const KernelInfo    kernel = { "range", kernel::range };
  const ParameterList params = { { "src", src },         { "dst", dst },         { "start_x", start_x },
                                 { "step_x", step_x },   { "start_y", start_y }, { "step_y", step_y },
                                 { "start_z", start_z }, { "step_z", step_z } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

// read_values_from_map_func

auto
read_values_from_positions_func(const Device::Pointer & device,
                                const Array::Pointer &  src,
                                const Array::Pointer &  list,
                                Array::Pointer          dst) -> Array::Pointer
{
  // TODO: check list shape
  // if (list->height() < src->dim())
  // {
  //   throw std::runtime_error("The list height is expected to be " + std::to_string(src->dim()) + ", but it is " +
  //                            std::to_string(list->width()));
  // }
  tier0::create_vector(src, dst, list->width());
  const KernelInfo    kernel = { "read_values_from_positions", kernel::read_values_from_positions };
  const ParameterList params = { { "src0", src }, { "src1", list }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
replace_values_func(const Device::Pointer & device,
                    const Array::Pointer &  src0,
                    const Array::Pointer &  src1,
                    Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src0, dst);
  const KernelInfo    kernel = { "replace_values", kernel::replace_values };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
replace_value_func(const Device::Pointer & device,
                   const Array::Pointer &  src,
                   Array::Pointer          dst,
                   float                   scalar0,
                   float                   scalar1) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "replace_value", kernel::replace_value };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar0", scalar0 }, { "scalar1", scalar1 } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

// resample_func
// touch_matrix_to_mesh_func

auto
maximum_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "maximum_sphere", kernel::maximum_sphere };
  const ParameterList params = { { "src", src },
                                 { "dst", dst },
                                 { "scalar0", radius2kernelsize(radius_x) },
                                 { "scalar1", radius2kernelsize(radius_y) },
                                 { "scalar2", radius2kernelsize(radius_z) } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
minimum_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "minimum_sphere", kernel::minimum_sphere };
  const ParameterList params = { { "src", src },
                                 { "dst", dst },
                                 { "scalar0", radius2kernelsize(radius_x) },
                                 { "scalar1", radius2kernelsize(radius_y) },
                                 { "scalar2", radius2kernelsize(radius_z) } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
multiply_matrix_func(const Device::Pointer & device,
                     const Array::Pointer &  src0,
                     const Array::Pointer &  src1,
                     Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_dst(src0, dst, src1->width(), src0->height(), src0->depth(), src0->dtype());
  const KernelInfo    kernel = { "multiply_matrix", kernel::multiply_matrix };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
reciprocal_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "reciprocal", kernel::reciprocal };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
set_func(const Device::Pointer & device, const Array::Pointer & src, float scalar) -> Array::Pointer
{
  const KernelInfo    kernel = { "set", kernel::set };
  const ParameterList params = { { "dst", src }, { "scalar", scalar } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return src;
}

auto
set_column_func(const Device::Pointer & device, const Array::Pointer & src, int column, float value) -> Array::Pointer
{
  const KernelInfo    kernel = { "set_column", kernel::set_column };
  const ParameterList params = { { "dst", src }, { "index", column }, { "scalar", value } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return src;
}

auto
set_image_borders_func(const Device::Pointer & device, const Array::Pointer & src, float value) -> Array::Pointer
{
  const KernelInfo    kernel = { "set_image_borders", kernel::set_image_borders };
  const ParameterList params = { { "dst", src }, { "scalar", value } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return src;
}

auto
set_plane_func(const Device::Pointer & device, const Array::Pointer & src, int plane, float value) -> Array::Pointer
{
  const KernelInfo    kernel = { "set_plane", kernel::set_plane };
  const ParameterList params = { { "dst", src }, { "index", plane }, { "scalar", value } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return src;
}

auto
set_ramp_x_func(const Device::Pointer & device, const Array::Pointer & src) -> Array::Pointer
{
  const KernelInfo    kernel = { "set_ramp_x", kernel::set_ramp_x };
  const ParameterList params = { { "dst", src } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return src;
}

auto
set_ramp_y_func(const Device::Pointer & device, const Array::Pointer & src) -> Array::Pointer
{
  const KernelInfo    kernel = { "set_ramp_y", kernel::set_ramp_y };
  const ParameterList params = { { "dst", src } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return src;
}

auto
set_ramp_z_func(const Device::Pointer & device, const Array::Pointer & src) -> Array::Pointer
{
  const KernelInfo    kernel = { "set_ramp_z", kernel::set_ramp_z };
  const ParameterList params = { { "dst", src } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return src;
}

auto
set_row_func(const Device::Pointer & device, const Array::Pointer & src, int row, float value) -> Array::Pointer
{
  const KernelInfo    kernel = { "set_row", kernel::set_row };
  const ParameterList params = { { "dst", src }, { "index", row }, { "scalar", value } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return src;
}

auto
set_nonzero_pixels_to_pixelindex_func(const Device::Pointer & device,
                                      const Array::Pointer &  src,
                                      Array::Pointer          dst,
                                      int                     offset) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "set_nonzero_pixels_to_pixelindex", kernel::set_nonzero_pixels_to_pixelindex };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "offset", offset } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
set_where_x_equals_y_func(const Device::Pointer & device, const Array::Pointer & src, float value) -> Array::Pointer
{
  const KernelInfo    kernel = { "set_where_x_equals_y", kernel::set_where_x_equals_y };
  const ParameterList params = { { "dst", src }, { "scalar", value } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return src;
}

auto
set_where_x_greater_than_y_func(const Device::Pointer & device, const Array::Pointer & src, float value)
  -> Array::Pointer
{
  const KernelInfo    kernel = { "set_where_x_greater_than_y", kernel::set_where_x_greater_than_y };
  const ParameterList params = { { "dst", src }, { "scalar", value } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return src;
}

auto
set_where_x_smaller_than_y_func(const Device::Pointer & device, const Array::Pointer & src, float value)
  -> Array::Pointer
{
  const KernelInfo    kernel = { "set_where_x_smaller_than_y", kernel::set_where_x_smaller_than_y };
  const ParameterList params = { { "dst", src }, { "scalar", value } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return src;
}

auto
sign_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  dst->fill(0);
  const KernelInfo    kernel = { "pixel_sign", kernel::sign };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
smaller_func(const Device::Pointer & device,
             const Array::Pointer &  src0,
             const Array::Pointer &  src1,
             Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src0, dst, dType::UINT8);
  const KernelInfo    kernel = { "smaller", kernel::smaller };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
smaller_constant_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::UINT8);
  const KernelInfo    kernel = { "smaller_constant", kernel::smaller_constant };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar", scalar } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
smaller_or_equal_func(const Device::Pointer & device,
                      const Array::Pointer &  src0,
                      const Array::Pointer &  src1,
                      Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src0, dst, dType::UINT8);
  const KernelInfo    kernel = { "smaller_or_equal", kernel::smaller_or_equal };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
smaller_or_equal_constant_func(const Device::Pointer & device,
                               const Array::Pointer &  src,
                               Array::Pointer          dst,
                               float                   scalar) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::UINT8);
  const KernelInfo    kernel = { "smaller_or_equal_constant", kernel::smaller_or_equal_constant };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar", scalar } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
sobel_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "sobel", kernel::sobel };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
square_root_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "square_root", kernel::square_root };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
std_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xy(src, dst);
  const KernelInfo    kernel = { "std_z_projection", kernel::std_z_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
subtract_image_from_scalar_func(const Device::Pointer & device,
                                const Array::Pointer &  src,
                                Array::Pointer          dst,
                                float                   scalar) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "subtract_image_from_scalar", kernel::subtract_image_from_scalar };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar", scalar } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
sum_reduction_x_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int blocksize)
  -> Array::Pointer
{
  // WARNING: function is not tested for 2D and 3D data. exepecting 1D only for now.
  if (dst == nullptr)
  {
    size_t dst_width = src->width();
    size_t dst_height = src->height();
    size_t dst_depth = src->depth();
    auto   dim = shape_to_dimension(dst_width, dst_height, dst_depth);
    switch (dim)
    {
      case 1:
        dst_width = static_cast<size_t>(src->width() / blocksize);
        break;
      case 2:
        dst_height = static_cast<size_t>(src->height() / blocksize);
        break;
      case 3:
        dst_depth = static_cast<size_t>(src->depth() / blocksize);
        break;
    }
    dst = Array::create(dst_width, dst_height, dst_depth, 1, src->dtype(), src->mtype(), src->device());
  }
  const KernelInfo    kernel = { "sum_reduction_x", kernel::sum_reduction_x };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "index", blocksize } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
sum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_zy(src, dst);
  const KernelInfo    kernel = { "sum_x_projection", kernel::sum_x_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
sum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xz(src, dst);
  const KernelInfo    kernel = { "sum_y_projection", kernel::sum_y_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
sum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xy(src, dst);
  const KernelInfo    kernel = { "sum_z_projection", kernel::sum_z_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

// auto
// superior_inferior(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
// {
//   tier0::create_like(src, dst, dType::UINT8);
//   if (src->dtype() != dType::UINT8)
//   {
//     throw std::runtime_error("inferior_superior only supports UINT8 images");
//   }
//   const KernelInfo    kernel = { "superior_inferior", kernel::superior_inferior };
//   const ParameterList params = { { "src", src }, { "dst", dst } };
//   const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
//   execute(device, kernel, params, range);
//   return dst;
// }

auto
transpose_xy_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  if (dst == nullptr)
  {
    auto dim = shape_to_dimension(src->height(), src->width(), src->depth());
    dst = Array::create(src->height(), src->width(), src->depth(), dim, src->dtype(), src->mtype(), src->device());
  }
  const KernelInfo    kernel = { "transpose_xy", kernel::transpose_xy };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
transpose_xz_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  if (dst == nullptr)
  {
    auto dim = shape_to_dimension(src->depth(), src->height(), src->width());
    dst = Array::create(src->depth(), src->height(), src->width(), dim, src->dtype(), src->mtype(), src->device());
  }
  const KernelInfo    kernel = { "transpose_xz", kernel::transpose_xz };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
transpose_yz_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  if (dst == nullptr)
  {
    auto dim = shape_to_dimension(src->width(), src->depth(), src->height());
    dst = Array::create(src->width(), src->depth(), src->height(), 3, src->dtype(), src->mtype(), src->device());
  }
  const KernelInfo    kernel = { "transpose_yz", kernel::transpose_yz };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
undefined_to_zero_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "undefined_to_zero", kernel::undefined_to_zero };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
variance_box_func(const Device::Pointer & device,
                  const Array::Pointer &  src,
                  Array::Pointer          dst,
                  int                     radius_x,
                  int                     radius_y,
                  int                     radius_z) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "variance_box", kernel::variance_box };
  const ParameterList params = { { "src", src },
                                 { "dst", dst },
                                 { "scalar0", radius2kernelsize(radius_x) },
                                 { "scalar1", radius2kernelsize(radius_y) },
                                 { "scalar2", radius2kernelsize(radius_z) } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
variance_sphere_func(const Device::Pointer & device,
                     const Array::Pointer &  src,
                     Array::Pointer          dst,
                     int                     radius_x,
                     int                     radius_y,
                     int                     radius_z) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "variance_sphere", kernel::variance_sphere };
  const ParameterList params = { { "src", src },
                                 { "dst", dst },
                                 { "scalar0", radius2kernelsize(radius_x) },
                                 { "scalar1", radius2kernelsize(radius_y) },
                                 { "scalar2", radius2kernelsize(radius_z) } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
write_values_to_positions_func(const Device::Pointer & device, const Array::Pointer & list, Array::Pointer dst)
  -> Array::Pointer
{
  // TODO: check list shape
  // if (list->dim() != 2)
  // {
  //   throw std::runtime_error(
  //     "The Position list is expected to be 2D, where rows are positions (x,y,z) and values v.");
  // }
  if (dst == nullptr)
  {
    // flatten the coords to get the max position value in x,y,z
    // as well as the number of rows (2->1D, 3->2D, 4->3D)
    auto temp = Array::create(1, list->height(), 1, 2, dType::INT32, list->mtype(), list->device());
    maximum_x_projection_func(device, list, temp);
    std::vector<int> max_position(temp->size());
    temp->read(max_position.data());
    size_t max_pos_x = max_position[0] + 1;
    size_t max_pos_y = (list->height() > 2) ? max_position[1] + 1 : 1;
    size_t max_pos_z = (list->height() > 3) ? max_position[2] + 1 : 1;
    auto   dim = shape_to_dimension(max_pos_x, max_pos_y, max_pos_z);
    dst = Array::create(max_pos_x, max_pos_y, max_pos_z, dim, list->dtype(), list->mtype(), list->device());
    dst->fill(0);
  }
  const KernelInfo    kernel = { "write_values_to_positions", kernel::write_values_to_positions };
  const ParameterList params = { { "src", list }, { "dst", dst } };
  const RangeArray    range = { list->width(), 1, 1 };
  execute(device, kernel, params, range);
  return dst;
}

auto
x_position_of_maximum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_zy(src, dst, dType::UINT32);
  const KernelInfo    kernel = { "x_position_of_maximum_x_projection", kernel::x_position_of_maximum_x_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
x_position_of_minimum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_zy(src, dst, dType::UINT32);
  const KernelInfo    kernel = { "x_position_of_minimum_x_projection", kernel::x_position_of_minimum_x_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
y_position_of_maximum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_xz(src, dst, dType::UINT32);
  const KernelInfo    kernel = { "y_position_of_maximum_y_projection", kernel::y_position_of_maximum_y_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
y_position_of_minimum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_xz(src, dst, dType::UINT32);
  const KernelInfo    kernel = { "y_position_of_minimum_y_projection", kernel::y_position_of_minimum_y_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
z_position_of_maximum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_xy(src, dst, dType::UINT32);
  const KernelInfo    kernel = { "z_position_of_maximum_z_projection", kernel::z_position_of_maximum_z_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
z_position_of_minimum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_xy(src, dst, dType::UINT32);
  const KernelInfo    kernel = { "z_position_of_minimum_z_projection", kernel::z_position_of_minimum_z_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
