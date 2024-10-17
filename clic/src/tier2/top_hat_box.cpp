#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

namespace cle::tier2
{

auto
top_hat_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 int                     radius_x,
                 int                     radius_y,
                 int                     radius_z) -> Array::Pointer
{
  std::cerr << "Deprecated: this function is deprecated, use top_hat instead\n";
  auto temp1 = tier1::minimum_filter_func(device, src, nullptr, radius_x, radius_y, radius_z, "box");
  auto temp2 = tier1::maximum_filter_func(device, temp1, nullptr, radius_x, radius_y, radius_z, "box");
  return tier1::add_images_weighted_func(device, src, temp2, dst, 1, -1);
}

} // namespace cle::tier2
