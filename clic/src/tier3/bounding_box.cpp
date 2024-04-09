#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp" 

#include "utils.hpp"

namespace cle::tier3 
{

auto
bounding_box_func(const Device::Pointer & device, const Array::Pointer & src) -> std::array<float, 6>
{
  float min_x = 0, min_y = 0, min_z = 0, max_x = 0, max_y = 0, max_z = 0;
  auto  temp = Array::create(
    src->width(), src->height(), src->depth(), src->dimension(), dType::INDEX, mType::BUFFER, src->device());
  tier1::multiply_image_and_position_func(device, src, temp, 0);
  max_x = tier2::maximum_of_all_pixels_func(device, temp);
  min_x = tier2::minimum_of_masked_pixels_func(device, temp, src);
  temp = tier1::multiply_image_and_position_func(device, src, nullptr, 1);
  max_y = tier2::maximum_of_all_pixels_func(device, temp);
  min_y = tier2::minimum_of_masked_pixels_func(device, temp, src);
  if (src->depth() > 1)
  {
    temp = tier1::multiply_image_and_position_func(device, src, nullptr, 2);
    max_z = tier2::maximum_of_all_pixels_func(device, temp);
    min_z = tier2::minimum_of_masked_pixels_func(device, temp, src);
  }
  return std::array<float, 6>{ min_x, min_y, min_z, max_x, max_y, max_z };
}  

}

