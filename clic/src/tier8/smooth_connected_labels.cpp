#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"
#include "tier6.hpp"
#include "tier7.hpp"
#include "tier8.hpp" 

#include "utils.hpp"

namespace cle::tier8 
{

auto
smooth_connected_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::LABEL);
  if (radius < 1)
  {
    return tier1::copy_func(device, src, dst);
  }
  auto binary = tier7::erode_connected_labels_func(device, src, nullptr, radius);
  return tier6::dilate_labels_func(device, binary, dst, radius);
}  

}

