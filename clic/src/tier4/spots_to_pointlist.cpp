#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp" 

#include "utils.hpp"

namespace cle::tier4 
{

auto
spots_to_pointlist_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  auto labeled_spots = tier2::label_spots_func(device, src, nullptr);
  return tier3::labelled_spots_to_pointlist_func(device, labeled_spots, dst);
}  

}

