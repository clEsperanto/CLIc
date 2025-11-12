#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_generate_distance_matrix.h"

namespace cle::tier1
{

auto
generate_distance_matrix_func(const Device::Pointer & device,
                              const Array::Pointer &  coordinate_list1,
                              const Array::Pointer &  coordinate_list2,
                              Array::Pointer          distance_matrix_destination) -> Array::Pointer
{
  tier0::create_dst(
    coordinate_list1, distance_matrix_destination, coordinate_list1->width() + 1, coordinate_list1->width() + 1, 1, dType::FLOAT);
  distance_matrix_destination->fill(0);
  const KernelInfo    kernel = { "generate_distance_matrix", kernel::generate_distance_matrix };
  const ParameterList params = { { "src0", coordinate_list1 }, { "src1", coordinate_list2 }, { "dst", distance_matrix_destination } };
  const RangeArray    range = { distance_matrix_destination->width(),
                                distance_matrix_destination->height(),
                                distance_matrix_destination->depth() };
  execute(device, kernel, params, range);
  return distance_matrix_destination;
}

} // namespace cle::tier1
