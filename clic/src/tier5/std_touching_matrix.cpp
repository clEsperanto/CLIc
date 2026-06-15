#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"

#include "utils.hpp"

namespace cle::tier5
{

auto
standard_deviation_partial_touching_area_matrix_func(const Device::Pointer & device,
                                                     const Array::Pointer &  src_label,
                                                     Array::Pointer          dst_vector) -> Array::Pointer
{
  auto partial_touching_area_matrix = tier4::generate_partial_touching_area_matrix_func(device, src_label, nullptr);
  auto touching_matrix = tier1::greater_constant_func(device, partial_touching_area_matrix, nullptr, 0.0f);
  touching_matrix = tier2::symmetric_minimum_matrix_func(device, touching_matrix, nullptr);
  touching_matrix = tier1::set_where_x_equals_y_func(device, touching_matrix, 0.0f);

  // count how many neigbors every label has, including background
  auto touch_count_vector = tier1::sum_y_projection_func(device, touching_matrix, nullptr);
  // the average touch portion is per definition 1 / neighbor_count
  auto mean_touch_ratio_vector = tier1::reciprocal_func(device, touch_count_vector, nullptr);

  // we turn this vector into a matrix where only touching objects are set
  auto mean_touching_matrix = tier1::multiply_images_func(device, touching_matrix, mean_touch_ratio_vector, nullptr);

  // subtracting the mean touch portion from the touch portion gives us the deviation from the average
  auto absolute_deviation_matrix = Array::create(partial_touching_area_matrix);
  evaluate(device, "abs(a - b)", { partial_touching_area_matrix, mean_touching_matrix }, absolute_deviation_matrix);

  // dividing it by count and summing over all labels gives us the standard deviation
  auto standard_deviation_matrix = tier1::divide_images_func(device, absolute_deviation_matrix, touch_count_vector, nullptr);
  dst_vector = tier1::nan_to_num_func(device, tier1::sum_y_projection_func(device, standard_deviation_matrix, nullptr), dst_vector);
  return dst_vector;
}

auto
standard_deviation_touch_portion_func(const Device::Pointer & device, const Array::Pointer & src_label, Array::Pointer dst_vector)
  -> Array::Pointer
{
  return standard_deviation_partial_touching_area_matrix_func(device, src_label, dst_vector);
}

} // namespace cle::tier5
