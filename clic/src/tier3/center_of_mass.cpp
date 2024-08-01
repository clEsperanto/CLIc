#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"

#include "utils.hpp"

namespace cle::tier3
{

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

} // namespace cle::tier3
