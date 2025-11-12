#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_write_values_to_positions.h"

namespace cle::tier1
{

auto
write_values_to_positions_func(const Device::Pointer & device, const Array::Pointer & list, Array::Pointer dst) -> Array::Pointer
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
    temp->readTo(max_position.data());
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

} // namespace cle::tier1
