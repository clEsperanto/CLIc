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
erode_connected_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::LABEL);
  if (radius < 1)
  {
    return tier1::copy_func(device, src, dst);
  }
  auto temp = tier1::greater_constant_func(device, src, nullptr, 0);
  auto eroded = tier6::erode_labels_func(device, temp, nullptr, radius, false);
  temp = tier1::multiply_images_func(device, src, eroded, nullptr);
  return tier4::relabel_sequential_func(device, temp, dst, 4096);
}

} // namespace cle::tier7
