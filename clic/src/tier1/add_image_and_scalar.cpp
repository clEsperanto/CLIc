#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_add_image_and_scalar.h"

namespace cle::tier1
{

auto
add_image_and_scalar_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel_code = { "add_image_and_scalar", kernel::add_image_and_scalar };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar", scalar } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel_code, params, range);
  return dst;
}

// average_distance_of_n_far_off_distances_func
// average_distance_of_n_far_off_distances_func
// average_distance_of_n_shortest_distances_func
// average_distance_of_n_shortest_distances_func
// average_distance_of_n_nearest_distances_func
// average_distance_of_touching_neighbors_func

} // namespace cle::tier1
