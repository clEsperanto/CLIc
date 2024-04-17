#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

namespace cle::tier2
{

auto
count_touching_neighbors_func(const Device::Pointer & device,
                              const Array::Pointer &  src,
                              Array::Pointer          dst,
                              bool                    ignore_background) -> Array::Pointer
{
  tier0::create_vector(src, dst, src->width(), dType::UINT32);
  auto bin_matrix = tier1::greater_constant_func(device, src, nullptr, 0);
  if (ignore_background)
  {
    tier1::set_row_func(device, bin_matrix, 0, 0);
    tier1::set_column_func(device, bin_matrix, 0, 0);
    tier1::set_where_x_equals_y_func(device, bin_matrix, 0);
  }
  return tier1::sum_y_projection_func(device, bin_matrix, dst);
}

} // namespace cle::tier2
