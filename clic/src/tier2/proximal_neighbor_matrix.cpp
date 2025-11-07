#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

namespace cle::tier2
{

auto
generate_proximal_neighbors_matrix_func(const Device::Pointer & device,
                                        const Array::Pointer &  src_matrix,
                                        Array::Pointer          dst_matrix,
                                        float                   min_distance,
                                        float                   max_distance) -> Array::Pointer
{
  tier0::create_like(src_matrix, dst_matrix, dType::UINT8);

  min_distance = std::max(min_distance, 0.0f);
  max_distance = (max_distance < 0) ? std::numeric_limits<float>::max() : max_distance;

  auto above_min_distance = tier1::greater_or_equal_constant_func(src_matrix, nullptr, min_distance);
  auto below_max_distance = tier1::smaller_or_equal_constant_func(src_matrix, nullptr, max_distance);

  tier1::binary_and_func(above_min_distance, below_max_distance, dst_matrix);
  tier1::set_where_x_equals_y_func(dst_matrix, 0);
  tier1::set_row_func(dst_matrix, 0, 0);
  tier1::set_column_func(dst_matrix, 0, 0);

  return dst_matrix;
}


} // namespace cle::tier2
