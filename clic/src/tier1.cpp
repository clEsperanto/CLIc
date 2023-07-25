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
#include "cle_count_touching_neighbors.h"
#include "cle_crop.h"
#include "cle_cubic_root.h"
#include "cle_detect_label_edges.h"
#include "cle_detect_maxima.h"
#include "cle_detect_minima.h"
#include "cle_dilate_box.h"
// #include "cle_dilate_box_slice_by_slice.h"
#include "cle_dilate_sphere.h"
// #include "cle_dilate_sphere_slice_by_slice.h"
#include "cle_divide_image_and_scalar.h"
#include "cle_divide_images.h"
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
// #include "cle_generate_distance_matrix.h"
// #include "cle_generate_touch_matrix.h"
#include "cle_gradient_x.h"
#include "cle_gradient_y.h"
#include "cle_gradient_z.h"
#include "cle_greater.h"
#include "cle_greater_constant.h"
#include "cle_greater_or_equal.h"
#include "cle_greater_or_equal_constant.h"
// #include "cle_hessian_eigenvalues.h"
#include "cle_laplace_box.h"
#include "cle_laplace_diamond.h"
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
// #include "cle_median_box.h"
// #include "cle_median_sphere.h"
#include "cle_minimum_separable.h"
// #include "cle_minimum_distance_of_touching_neighbors.h"
#include "cle_minimum_image_and_scalar.h"
#include "cle_minimum_images.h"
#include "cle_minimum_x_projection.h"
#include "cle_minimum_y_projection.h"
#include "cle_minimum_z_projection.h"
// #include "cle_mode_box.h"
// #include "cle_mode_sphere.h"
#include "cle_modulo_images.h"
#include "cle_multiply_image_and_coordinate.h"
#include "cle_multiply_image_and_scalar.h"
#include "cle_multiply_images.h"
// #include "cle_n_closest_points.h"
// #include "cle_nan_to_num.h"
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
// #include "cle_read_intensities_from_map.h"
// #include "cle_read_intensities_from_positions.h"
#include "cle_replace_intensities.h"
#include "cle_replace_intensity.h"
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
// #include "cle_standard_deviation_z_projection.h"
#include "cle_subtract_image_from_scalar.h"
#include "cle_sum_reduction_x.h"
#include "cle_sum_x_projection.h"
#include "cle_sum_y_projection.h"
#include "cle_sum_z_projection.h"
#include "cle_transpose_xy.h"
#include "cle_transpose_xz.h"
#include "cle_transpose_yz.h"
#include "cle_undefined_to_zero.h"
#include "cle_variance_box.h"
#include "cle_variance_sphere.h"
#include "cle_write_values_to_positions.h"


namespace cle::tier1
{

auto
absolute_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "absolute", kernel::absolute };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
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
  tier0::create_like(src0, dst);
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
  tier0::create_like(src0, dst);
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
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "binary_edge_detection", kernel::binary_edge_detection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
binary_not_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
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
  tier0::create_like(src0, dst);
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
  tier0::create_like(src0, dst);
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
  tier0::create_like(src0, dst);
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
  if (dst->dim() == 3)
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
  if (dst->dim() == 3)
  {
    kernel = { "copy_horizontal_slice_to", kernel::copy_horizontal_slice_to };
    range = { src->width(), src->height(), 1 };
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
  if (dst->dim() == 3)
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
          int                     witdh,
          int                     height,
          int                     depth) -> Array::Pointer
{
  tier0::create_dst(src, dst, witdh, height, depth);
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
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "detect_label_edges", kernel::detect_label_edges };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
detect_maxima_box_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "detect_maxima", kernel::detect_maxima };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
detect_minima_box_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "detect_minima", kernel::detect_minima };
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

// dilate_box_slice_by_slice_func

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
divide_image_and_scalar_func(const Device::Pointer & device,
                             const Array::Pointer &  src,
                             Array::Pointer          dst,
                             float                   scalar) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "divide_image_and_scalar", kernel::divide_image_and_scalar };
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
  tier0::create_like(src0, dst);
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
  tier0::create_like(src, dst);
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
// generate_distance_matrix_func
// generate_touch_matrix_func

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
  tier0::create_like(src0, dst);
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
  tier0::create_like(src, dst);
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
  tier0::create_like(src0, dst);
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
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "greater_or_equal_constant", kernel::greater_or_equal_constant };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar", scalar } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

// hessian_eigenvalues_func

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
  tier0::create_zy(src, dst);
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
  tier0::create_xz(src, dst);
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
  tier0::create_xy(src, dst);
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

// median_box_func
// median_sphere_func

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

// mode_box_func
// mode_sphere_func

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

// multiply_image_and_coordinate_func

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
// nan_to_num_func

auto
nonzero_maximum_box_func(const Device::Pointer & device,
                         const Array::Pointer &  src,
                         Array::Pointer          dst0,
                         Array::Pointer          dst1) -> Array::Pointer
{
  tier0::create_like(src, dst1);
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
  tier0::create_like(src0, dst);
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
  tier0::create_like(src, dst);
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
           int                     start_y,
           int                     start_z,
           int                     step_x,
           int                     step_y,
           int                     step_z) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "range", kernel::range };
  const ParameterList params = { { "src", src },         { "dst", dst },         { "start_x", start_x },
                                 { "start_y", start_y }, { "start_z", start_z }, { "step_x", step_x },
                                 { "step_y", step_y },   { "step_z", step_z } };
  const RangeArray    range = { 1, 1, 1 };
  execute(device, kernel, params, range);
  return dst;
}

// read_intensities_from_map_func
// read_intensities_from_positions_func

auto
replace_intensities_func(const Device::Pointer & device,
                         const Array::Pointer &  src0,
                         const Array::Pointer &  src1,
                         Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src0, dst);
  const KernelInfo    kernel = { "replace_intensities", kernel::replace_intensities };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
replace_intensity_func(const Device::Pointer & device,
                       const Array::Pointer &  src,
                       Array::Pointer          dst,
                       float                   scalar0,
                       float                   scalar1) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "replace_intensity", kernel::replace_intensity };
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
                    int                     radius_x,
                    int                     radius_y,
                    int                     radius_z) -> Array::Pointer
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
                    int                     radius_x,
                    int                     radius_y,
                    int                     radius_z) -> Array::Pointer
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

// multiply_matrix_func

auto
reciprocal_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
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

// set_image_borders_func

auto
set_plane_func(const Device::Pointer & device, const Array::Pointer & src, int plane, float value) -> Array::Pointer
{
  const KernelInfo    kernel = { "set_plane", kernel::set_plane };
  const ParameterList params = { { "dst", src }, { "index", plane }, { "scalar", value } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return src;
}

// set_ramp_x_func
// set_ramp_y_func
// set_ramp_z_func

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
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "set_nonzero_pixels_to_pixelindex", kernel::set_nonzero_pixels_to_pixelindex };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "offset", offset } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

// set_where_x_equals_y_func
// set_where_x_greater_than_y_func
// set_where_x_smaller_than_y_func

auto
sign_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "sign", kernel::sign };
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
  tier0::create_like(src0, dst);
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
  tier0::create_like(src, dst);
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
  tier0::create_like(src0, dst);
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
  tier0::create_like(src, dst);
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

// standard_deviation_z_projection_func

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
  if (dst == nullptr)
  {
    size_t dst_width = src->width();
    size_t dst_height = src->height();
    size_t dst_depth = src->depth();
    switch (src->dim())
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
    dst = Array::create(dst_width, dst_height, dst_depth, src->dtype(), src->mtype(), src->device());
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

auto
transpose_xy_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  if (dst == nullptr)
  {
    dst = Array::create(src->height(), src->width(), src->depth(), src->dtype(), src->mtype(), src->device());
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
    dst = Array::create(src->depth(), src->height(), src->width(), src->dtype(), src->mtype(), src->device());
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
    dst = Array::create(src->width(), src->depth(), src->height(), src->dtype(), src->mtype(), src->device());
  }
  const KernelInfo    kernel = { "transpose_yz", kernel::transpose_yz };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

// undefined_to_zero_func

auto
variance_box_func(const Device::Pointer & device,
                  const Array::Pointer &  src,
                  Array::Pointer          dst,
                  int                     radius_x,
                  int                     radius_y,
                  int                     radius_z) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "variance_box", kernel::variance_box };
  const ParameterList params = { { "src", src },
                                 { "dst", dst },
                                 { "index0", radius2kernelsize(radius_x) },
                                 { "index1", radius2kernelsize(radius_y) },
                                 { "index2", radius2kernelsize(radius_z) } };
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
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "variance_sphere", kernel::variance_sphere };
  const ParameterList params = { { "src", src },
                                 { "dst", dst },
                                 { "index0", radius2kernelsize(radius_x) },
                                 { "index1", radius2kernelsize(radius_y) },
                                 { "index2", radius2kernelsize(radius_z) } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
write_values_to_positions_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  if (src->dim() != 2)
  {
    throw std::runtime_error("Source image is expected to be 2D, where rows are coordinates (x,y,z) and values v.");
  }
  if (dst == nullptr)
  {
    Array::Pointer temp = nullptr;
    tier0::create_zy(src, temp, dType::INT32);
    maximum_x_projection_func(device, src, temp);
    auto nb_max_position = temp->nbElements() - 1;

    std::vector<int> max_position(temp->nbElements());
    temp->read(max_position.data());

    size_t max_pos_x = max_position[0];
    size_t max_pos_y = (nb_max_position > 2) ? max_position[1] : 1;
    size_t max_pos_z = (nb_max_position > 3) ? max_position[2] : 1;
    dst = Array::create(max_pos_x, max_pos_y, max_pos_z, src->dtype(), src->mtype(), src->device());
  }
  const KernelInfo    kernel = { "write_values_to_positions", kernel::write_values_to_positions };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
