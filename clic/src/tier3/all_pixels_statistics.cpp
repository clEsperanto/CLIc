#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "utils.hpp"

namespace cle::tier3
{

auto
mean_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float
{
  auto temp = tier2::sum_of_all_pixels_func(device, src);
  return temp / src->size();
}

auto
center_of_mass_func(const Device::Pointer & device, const Array::Pointer & src) -> std::vector<float>
{
  auto sum = tier2::sum_of_all_pixels_func(device, src);
  auto temp = tier1::multiply_image_and_position_func(device, src, nullptr, 0);
  auto sum_x = tier2::sum_of_all_pixels_func(device, temp);
  temp = tier1::multiply_image_and_position_func(device, src, nullptr, 1);
  auto sum_y = tier2::sum_of_all_pixels_func(device, temp);
  temp = tier1::multiply_image_and_position_func(device, src, nullptr, 2);
  auto sum_z = tier2::sum_of_all_pixels_func(device, temp);
  return std::vector<float>{ sum_x / sum, sum_y / sum, sum_z / sum };
}

auto
bounding_box_func(const Device::Pointer & device, const Array::Pointer & src) -> std::vector<float>
{
  float min_x = 0, min_y = 0, min_z = 0, max_x = 0, max_y = 0, max_z = 0;
  auto  temp = tier1::multiply_image_and_position_func(device, src, nullptr, 0);
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
  return std::vector<float>{ min_x, min_y, min_z, max_x, max_y, max_z };
}

} // namespace cle::tier3
