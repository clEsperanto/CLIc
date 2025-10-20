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

  Array::Pointer middle_eigenvalue = nullptr;
  auto           large_eigenvalue =
    Array::create(src->width(), src->height(), src->depth(), src->dim(), dType::FLOAT, mType::BUFFER, device);
  if (is_3d)
  {
    middle_eigenvalue =
      Array::create(src->width(), src->height(), src->depth(), src->dim(), dType::FLOAT, mType::BUFFER, device);
  }

  for (int sigma = static_cast<int>(sigma_minimum); sigma < static_cast<int>(sigma_maximum);
       sigma += static_cast<int>(sigma_step))
  {
    tier2::hessian_gaussian_eigenvalues_func(device, src, nullptr, middle_eigenvalue, large_eigenvalue, sigma);

    float max = tier2::maximum_of_all_pixels_func(device, large_eigenvalue);
    tier2::clip_func(device, large_eigenvalue, large_eigenvalue, 0.0f, max);

    auto mean_eigenvalue = large_eigenvalue;
    if (is_3d)
    {
      max = tier2::maximum_of_all_pixels_func(device, middle_eigenvalue);
      tier2::clip_func(device, middle_eigenvalue, middle_eigenvalue, 0.0f, max);
      mean_eigenvalue = tier1::multiply_images_func(device, middle_eigenvalue, large_eigenvalue, nullptr);
      tier1::power_func(device, mean_eigenvalue, nullptr, 0.5f);
    }

    float sigma_squared = static_cast<float>(sigma * sigma);
    auto  value = tier1::multiply_image_and_scalar_func(device, mean_eigenvalue, nullptr, sigma_squared);

    tier1::maximum_images_func(device, value, dst, dst);
  }

  return dst;
}

auto
tubeness_func(const Device::Pointer & device,
              const Array::Pointer &  src,
              Array::Pointer          dst,
              float                   sigma) -> Array::Pointer
              {
  tier0::create_like(src, dst);
  dst->fill(0.0f);

  auto is_3d = (src->depth() > 1);

  Array::Pointer temp = nullptr;
  Array::Pointer middle_eigenvalue = nullptr;
  auto           small_eigenvalue =
    Array::create(src->width(), src->height(), src->depth(), src->dim(), dType::FLOAT, mType::BUFFER, device);
  if (is_3d)
  {
    middle_eigenvalue =
      Array::create(src->width(), src->height(), src->depth(), src->dim(), dType::FLOAT, mType::BUFFER, device);
  }
  tier2::hessian_gaussian_eigenvalues_func(device, src, small_eigenvalue, middle_eigenvalue, nullptr, sigma);

  auto sigma_squared = sigma * sigma;
  auto min_small = tier2::minimum_of_all_pixels_func(device, small_eigenvalue);
  tier2::clip_func(device, small_eigenvalue, small_eigenvalue, min_small, 0.0f);
  if (is_3d)
  {
    auto min_middle = tier2::minimum_of_all_pixels_func(device, middle_eigenvalue);
    tier2::clip_func(device, middle_eigenvalue, middle_eigenvalue, min_middle, 0.0f);
    temp = tier1::power_func(device, 
      tier1::multiply_images_func(device, middle_eigenvalue, small_eigenvalue, nullptr), 
      nullptr, 0.5f);
    tier1::multiply_image_and_scalar_func(device, temp, dst, sigma_squared);
  }
  else
  {
    temp = tier1::absolute_func(device, small_eigenvalue, dst);
  }
  
  tier1::multiply_image_and_scalar_func(device, temp, dst, sigma_squared);
  return dst;
}

} // namespace cle::tier3
