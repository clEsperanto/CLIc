#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

namespace cle::tier2
{

auto
clip_func(const Device::Pointer & device,
          const Array::Pointer &  src,
          Array::Pointer          dst,
          float                   min_intensity,
          float                   max_intensity) -> Array::Pointer
{
  auto temp = tier1::maximum_image_and_scalar_func(device, src, nullptr, min_intensity);
  return tier1::minimum_image_and_scalar_func(device, temp, dst, max_intensity);
}

} // namespace cle::tier2
