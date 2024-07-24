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
eroded_otsu_labeling_func(const Device::Pointer & device,
                          const Array::Pointer &  src,
                          Array::Pointer          dst,
                          int                     number_of_erosions,
                          float                   outline_sigma) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::LABEL);
  auto           blurred = tier1::gaussian_blur_func(device, src, nullptr, outline_sigma, outline_sigma, outline_sigma);
  auto           binary = tier4::threshold_otsu_func(device, blurred, nullptr);
  Array::Pointer eroded1 = nullptr;
  Array::Pointer eroded2 = nullptr;
  tier0::create_like(binary, eroded1);
  tier0::create_like(binary, eroded2);
  binary->copyTo(eroded1);
  for (int i = 0; i < number_of_erosions; i++)
  {
    tier1::erode_box_func(device, eroded1, eroded2);
    std::swap(eroded1, eroded2);
  }
  return tier6::masked_voronoi_labeling_func(device, eroded1, binary, dst);
}

} // namespace cle::tier7
