#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

namespace cle::tier2
{

auto
count_touching_neighbors_func(const Device::Pointer & device,
                              const Array::Pointer &  touch_matrix,
                              Array::Pointer          touching_neighbors_count_destination,
                              bool                    ignore_background) -> Array::Pointer
{
  tier0::create_vector(touch_matrix, touching_neighbors_count_destination, touch_matrix->width(), dType::UINT32);
  auto bin_matrix = tier1::greater_constant_func(device, touch_matrix, nullptr, 0);
  if (ignore_background)
  {
    tier1::set_row_func(device, bin_matrix, 0, 0);
    tier1::set_column_func(device, bin_matrix, 0, 0);
    tier1::set_where_x_equals_y_func(device, bin_matrix, 0);
  }
  return tier1::sum_y_projection_func(device, bin_matrix, touching_neighbors_count_destination);
}


auto
symmetric_maximum_matrix_func(const Device::Pointer & device, const Array::Pointer & src_matrix, Array::Pointer dst_matrix)
  -> Array::Pointer
{
  auto temp = tier1::transpose_xy_func(device, src_matrix, nullptr);
  dst_matrix = tier1::maximum_images_func(device, src_matrix, temp, dst_matrix);
  return dst_matrix;
}

auto
symmetric_minimum_matrix_func(const Device::Pointer & device, const Array::Pointer & src_matrix, Array::Pointer dst_matrix)
  -> Array::Pointer
{
  auto temp = tier1::transpose_xy_func(device, src_matrix, nullptr);
  dst_matrix = tier1::minimum_images_func(device, src_matrix, temp, dst_matrix);
  return dst_matrix;
}

auto
symmetric_mean_matrix_func(const Device::Pointer & device, const Array::Pointer & src_matrix, Array::Pointer dst_matrix) -> Array::Pointer
{
  auto temp = tier1::transpose_xy_func(device, src_matrix, nullptr);
  tier0::create_like(src_matrix, dst_matrix);
  evaluate(device, "(a + b) / 2", { src_matrix, temp }, dst_matrix);
  return dst_matrix;
}

auto
generate_partial_touching_area_matrix_within_range_func(const Device::Pointer & device,
                                                const Array::Pointer &  src_matrix,
                                                Array::Pointer          dst_matrix,
                                                float                   min_distance,
                                                float                   max_distance) -> Array::Pointer
{
  tier0::create_like(src_matrix, dst_matrix);
  auto sup = tier1::greater_constant_func(device, src_matrix, nullptr, min_distance);
  auto inf = tier1::smaller_constant_func(device, src_matrix, nullptr, max_distance);
  tier1::binary_and_func(device, sup, inf, dst_matrix);
  return dst_matrix;
}

auto
generate_touch_portion_within_range_neighbors_matrix_func(const Device::Pointer & device,
                                                          const Array::Pointer &  src_matrix,
                                                          Array::Pointer          dst_matrix,
                                                          float                   min_distance,
                                                          float                   max_distance) -> Array::Pointer
{
  return generate_partial_touching_area_matrix_within_range_func(device, src_matrix, dst_matrix, min_distance, max_distance);
}

} // namespace cle::tier2
