#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

namespace cle::tier2
{

auto
top_hat_func(const Device::Pointer & device,
             const Array::Pointer &  src,
             Array::Pointer          dst,
             float                   radius_x,
             float                   radius_y,
             float                   radius_z,
             std::string             connectivity) -> Array::Pointer
{
  auto temp1 = tier1::minimum_filter_func(device, src, nullptr, radius_x, radius_y, radius_z, connectivity);
  auto temp2 = tier1::maximum_filter_func(device, temp1, nullptr, radius_x, radius_y, radius_z, connectivity);
  return tier1::add_images_weighted_func(device, src, temp2, dst, 1, -1);
}

auto
top_hat_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 float                   radius_x,
                 float                   radius_y,
                 float                   radius_z) -> Array::Pointer
{
  return top_hat_func(device, src, dst, radius_x, radius_y, radius_z, "box");
}


auto
top_hat_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z) -> Array::Pointer
{
  return top_hat_func(device, src, dst, radius_x, radius_y, radius_z, "sphere");
}

} // namespace cle::tier2
