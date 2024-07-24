#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

namespace cle::tier2
{

auto
radians_to_degrees_func(const Device::Pointer & device,
                        const Array::Pointer &  src,
                        Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  return tier1::multiply_image_and_scalar_func(device, src, dst, 180.0 / static_cast<float>(M_PI));
}

} // namespace cle::tier2
