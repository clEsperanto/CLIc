#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"
#include "tier6.hpp"

#include "utils.hpp"

namespace cle::tier6
{

auto
gauss_otsu_labeling_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float outline_sigma)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::LABEL);
  auto temp = tier1::gaussian_blur_func(device, src, nullptr, outline_sigma, outline_sigma, outline_sigma);
  auto binary = tier4::threshold_otsu_func(device, temp, nullptr);
  tier5::connected_component_labeling_func(device, binary, dst, "box");
  return dst;
}

} // namespace cle::tier6
