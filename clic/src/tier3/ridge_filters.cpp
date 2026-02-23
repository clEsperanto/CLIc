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
  auto small_eigenvalue = Array::create(src->width(), src->height(), src->depth(), src->dim(), dType::FLOAT, mType::BUFFER, device);
  if (is_3d)
  {
    middle_eigenvalue = Array::create(src->width(), src->height(), src->depth(), src->dim(), dType::FLOAT, mType::BUFFER, device);
  }

  for (float sigma = sigma_minimum; sigma < sigma_maximum; sigma += sigma_step)
  {
    float sigma_squared = static_cast<float>(sigma * sigma);
    tier2::hessian_gaussian_eigenvalues_func(device, src, small_eigenvalue, middle_eigenvalue, nullptr, sigma);

    if (is_3d)
    {
      evaluate(device, "fmax(sqrt(fmin(a, 0.0f) * fmin(b, 0.0f)) * s, out)", {middle_eigenvalue, small_eigenvalue, sigma_squared, dst}, dst);
    }
    else
    {
      evaluate(device, "fmax(fabs(fmin(a, 0.0f)) * s, out)", {small_eigenvalue, sigma_squared, dst}, dst);
    }
  }

  return dst;
}

auto
tubeness_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float sigma) -> Array::Pointer
{
  return sato_filter_func(device, src, dst, sigma, sigma + 0.1f, 0.1f);
}

} // namespace cle::tier3
