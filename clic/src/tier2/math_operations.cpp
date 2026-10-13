#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "utils.hpp"

namespace cle::tier2
{

auto
absolute_difference_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_like(src0, dst, promoteType(src0->dtype(), src1->dtype()));

  evaluate(device, "fabs(src0 - src1)", { src0, src1 }, dst);
  return dst;
}

auto
add_images_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  return tier1::add_images_weighted_func(device, src0, src1, dst, 1, 1);
}

auto
subtract_images_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  return tier1::add_images_weighted_func(device, src0, src1, dst, 1, -1);
}

auto
squared_difference_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_like(src0, dst, dType::FLOAT);
  evaluate(device, "pow(src0 - src1, 2)", { src0, src1 }, dst);
  return dst;
}

auto
square_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return tier1::power_func(device, src, dst, 2);
}

auto
invert_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return tier1::multiply_image_and_scalar_func(device, src, dst, -1);
}

auto
clip_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float min_intensity, float max_intensity)
  -> Array::Pointer
{

  tier0::create_like(src, dst);
  min_intensity = std::isnan(min_intensity) ? tier1::minimum_of_all_pixels_func(device, src) : min_intensity;
  max_intensity = std::isnan(max_intensity) ? tier1::maximum_of_all_pixels_func(device, src) : max_intensity;
  evaluate(device, "clamp(a, lo, hi)", { src, min_intensity, max_intensity }, dst);
  return dst;
}

auto
degrees_to_radians_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  return tier1::multiply_image_and_scalar_func(device, src, dst, static_cast<float>(M_PI) / 180.0);
}

auto
radians_to_degrees_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  return tier1::multiply_image_and_scalar_func(device, src, dst, 180.0 / static_cast<float>(M_PI));
}

} // namespace cle::tier2
