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
           int                   sigma_minimum,
           int                   sigma_maximum,
           int                   sigma_step) -> Array::Pointer
{
  tier0::create_like(src, dst);
  auto is_3d = (src->depth() > 1);

  auto large_eigenvalue = tier0::create_like(src, dType::FLOAT);
  Array::Pointer middle_eigenvalue = nullptr;
  if (is_3d)
  {
    middle_eigenvalue = tier0::create_like(src, dType::FLOAT);
  }

  float max = 0.0f;
  float sigma_squared = 0.0f;
  for(int sigma = sigma_minimum; sigma < sigma_maximum; sigma += sigma_step)
  {
    sigma_squared = static_cast<float>(sigma * sigma);
    tier2::hessian_gaussian_eigenvalues_func(device, src, nullptr, middle_eigenvalue, large_eigenvalue, sigma);

    max = tier2::maximum_of_all_pixels_func(device, large_eigenvalue);
    tier2::clip_func(device, large_eigenvalue, large_eigenvalue, 0.0f, max);
    
    auto mean_eigenvalue = large_eigenvalue;
    if (is_3d)
    {
      max = tier2::maximum_of_all_pixels_func(device, middle_eigenvalue);
      tier2::clip_func(device, middle_eigenvalue, middle_eigenvalue, 0.0f, max);
      mean_eigenvalue = tier1::multiply_images_func(device, middle_eigenvalue, large_eigenvalue, nullptr);
      tier1::power_func(device, mean_eigenvalue, nullptr, 0.5f);
    }
    auto value = tier1::multiply_by_scalar_func(device, mean_eigenvalue, nullptr, sigma_squared);
    tier1::maximum_images_func(device, value, dst, dst);
  }

  return dst;
}

} // namespace cle::tier3
