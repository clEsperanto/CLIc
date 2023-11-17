
#include "tier7.hpp"
#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"
#include "tier6.hpp"

namespace cle::tier7
{

auto
closing_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::UINT32);
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
      tier1::erode_sphere_func(device, flip, flop);
    }
    else
    {
      tier1::erode_box_func(device, flip, flop);
    }
  }
  if (radius % 2 == 0)
  {
    return tier1::multiply_images_func(device, flip, temp, dst);
  }
  return tier1::multiply_images_func(device, flop, temp, dst);
}

auto
opening_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::UINT32);
  auto temp = tier6::erode_labels_func(device, src, nullptr, radius, true);
  return tier6::dilate_labels_func(device, temp, dst, radius);
}

auto
voronoi_otsu_labeling_func(const Device::Pointer & device,
                           const Array::Pointer &  src,
                           Array::Pointer          dst,
                           float                   spot_sigma,
                           float                   outline_sigma) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::UINT32);
  auto temp = tier1::gaussian_blur_func(device, src, nullptr, spot_sigma, spot_sigma, spot_sigma);
  auto spot = tier1::detect_maxima_box_func(device, temp, nullptr);
  temp = tier1::gaussian_blur_func(device, src, nullptr, outline_sigma, outline_sigma, outline_sigma);
  auto segmentation = tier4::threshold_otsu_func(device, temp, nullptr);
  auto binary = tier1::binary_and_func(device, spot, segmentation, nullptr);
  temp = tier6::masked_voronoi_labeling_func(device, binary, segmentation, nullptr);
  return tier1::mask_func(device, temp, segmentation, dst);
}


} // namespace cle::tier7
