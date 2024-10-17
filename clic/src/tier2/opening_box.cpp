#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

namespace cle::tier2
{

auto
opening_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 int                     radius_x,
                 int                     radius_y,
                 int                     radius_z) -> Array::Pointer
{
  std::cerr << "Deprecated: this function is deprecated, use opening instead\n";
  auto temp = tier1::minimum_filter_func(device, src, nullptr, radius_x, radius_y, radius_z, "box");
  return tier1::maximum_filter_func(device, temp, dst, radius_x, radius_y, radius_z, "box");
}

} // namespace cle::tier2
