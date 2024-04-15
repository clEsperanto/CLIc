#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

namespace cle::tier2
{

auto
crop_border_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int border_size)
  -> Array::Pointer
{
  std::array<int, 3> region = { static_cast<int>(src->width()) - 2 * border_size,
                                static_cast<int>(src->height()) - 2 * border_size,
                                static_cast<int>(src->depth()) - 2 * border_size };
  std::transform(region.begin(), region.end(), region.begin(), [](int i) { return std::max(i, 0); });
  return tier1::crop_func(device, src, dst, border_size, border_size, border_size, region[0], region[1], region[2]);
}

// @StRigaud TODO: auto distance_matrix_to_mesh_func;

} // namespace cle::tier2
