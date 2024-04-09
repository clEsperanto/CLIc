#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_maximum_separable.h"

namespace cle::tier1
{

auto
maximum_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 int                     radius_x,
                 int                     radius_y,
                 int                     radius_z) -> Array::Pointer
{
  std::cerr << "Deprecated: Please use maximum() instead\n";
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

} // namespace cle::tier1
