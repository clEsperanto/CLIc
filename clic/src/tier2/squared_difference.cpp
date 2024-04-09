#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp" 

#include "utils.hpp"

namespace cle::tier2 
{

auto
squared_difference_func(const Device::Pointer & device,
                        const Array::Pointer &  src0,
                        const Array::Pointer &  src1,
                        Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src0, dst, dType::FLOAT);
  auto tmp = tier1::add_images_weighted_func(device, src0, src1, nullptr, 1, -1);
  return tier1::power_func(device, tmp, dst, 2);
}  

}

