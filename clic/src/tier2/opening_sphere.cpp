#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp" 

#include "utils.hpp"

namespace cle::tier2 
{

auto
opening_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z) -> Array::Pointer
{
  std::cerr << "Deprecated: this function is deprecated, use opening instead\n";
  auto temp = tier1::minimum_sphere_func(device, src, nullptr, radius_x, radius_y, radius_z);
  return tier1::maximum_sphere_func(device, temp, dst, radius_x, radius_y, radius_z);
}  

}

