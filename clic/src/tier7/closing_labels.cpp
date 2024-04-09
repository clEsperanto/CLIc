#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"
#include "tier6.hpp"
#include "tier7.hpp" 

#include "utils.hpp"

namespace cle::tier7 
{

auto
closing_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::LABEL);
  if (radius == 0)
  {
    return tier1::copy_func(device, src, dst);
  }

  auto temp = tier6::dilate_labels_func(device, src, nullptr, radius);

  auto flip = tier1::greater_constant_func(device, temp, nullptr, 0);
  auto flop = Array::create(flip);

  for (size_t i = 0; i < radius; i++)
  {
    if (i % 2 == 0)
    {
      tier1::erode_func(device, flip, flop, "sphere");
    }
    else
    {
      tier1::erode_func(device, flip, flop, "box");
    }
  }
  if (radius % 2 == 0)
  {
    return tier1::multiply_images_func(device, flip, temp, dst);
  }
  return tier1::multiply_images_func(device, flop, temp, dst);
}  

}

