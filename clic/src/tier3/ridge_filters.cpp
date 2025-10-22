#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"

#include "utils.hpp"

namespace cle::tier3
{

auto
sato_filter_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 float                   sigma_minimum,
                 float                   sigma_maximum,
                 float                   sigma_step) -> Array::Pointer
{
  tier0::create_like(src, dst);
  dst->fill(0.0f);

  auto is_3d = (src->depth() > 1);

  Array::Pointer temporary = nullptr;
  Array::Pointer middle_eigenvalue = nullptr;
  auto           small_eigenvalue =
    Array::create(src->width(), src->height(), src->depth(), src->dim(), dType::FLOAT, mType::BUFFER, device);
  if (is_3d)
  {
    middle_eigenvalue =
      Array::create(src->width(), src->height(), src->depth(), src->dim(), dType::FLOAT, mType::BUFFER, device);
  }

  for (float sigma = sigma_minimum; sigma < sigma_maximum; sigma += sigma_step)
  {
    tier2::hessian_gaussian_eigenvalues_func(device, src, small_eigenvalue, middle_eigenvalue, nullptr, sigma);

    float min = tier2::minimum_of_all_pixels_func(device, small_eigenvalue);
    tier2::clip_func(device, small_eigenvalue, small_eigenvalue, min, 0.0f);
    if (is_3d)
    {
      min = tier2::minimum_of_all_pixels_func(device, middle_eigenvalue);
      tier2::clip_func(device, middle_eigenvalue, middle_eigenvalue, min, 0.0f);

      auto multiply = tier1::multiply_images_func(device, middle_eigenvalue, small_eigenvalue, nullptr);
      temporary = tier1::power_func(device, multiply, nullptr, 0.5f);
    }
    else
    {
      temporary = tier1::absolute_func(device, small_eigenvalue, nullptr);
    }

    float sigma_squared = static_cast<float>(sigma * sigma);
    auto  value = tier1::multiply_image_and_scalar_func(device, temporary, nullptr, sigma_squared);
    tier1::maximum_images_func(device, value, dst, dst);
  }

  return dst;
}

auto
tubeness_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float sigma)
  -> Array::Pointer
{
  return sato_filter_func(device, src, dst, sigma, sigma + 0.1f, 0.1f);
}

} // namespace cle::tier3
