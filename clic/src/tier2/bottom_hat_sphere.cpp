#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp" 

#include "utils.hpp"

namespace cle::tier2 
{

auto
bottom_hat_sphere_func(const Device::Pointer & device,
                       const Array::Pointer &  src,
                       Array::Pointer          dst,
                       float                   radius_x,
                       float                   radius_y,
                       float                   radius_z) -> Array::Pointer
{
  std::cerr << "Deprecated: this function is deprecated, use bottom_hat instead\n";
  auto temp1 = tier1::maximum_sphere_func(device, src, nullptr, radius_x, radius_y, radius_z);
  auto temp2 = tier1::minimum_sphere_func(device, temp1, nullptr, radius_x, radius_y, radius_z);
  return tier1::add_images_weighted_func(device, temp2, src, dst, 1, -1);
}  

}

