#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_hessian_gaussian_eigenvalues.h"

namespace cle::tier2
{

auto
hessian_gaussian_eigenvalues_func(const Device::Pointer & device,
                         const Array::Pointer &  src,
                         Array::Pointer          small_eigenvalue,
                         Array::Pointer          middle_eigenvalue,
                         Array::Pointer          large_eigenvalue,
                        float sigma) -> std::vector<Array::Pointer>
{
  // TODO: check when src is 1D
  tier0::create_like(src, small_eigenvalue, dType::FLOAT);
  tier0::create_like(src, large_eigenvalue, dType::FLOAT);
  if (src->depth() > 1)
  {
    tier0::create_like(src, middle_eigenvalue, dType::FLOAT);
  }
  else
  {
    // no middle eigenvalue for 2D images, we replace the image by a scalar to save memory
    tier0::create_one(src, middle_eigenvalue, dType::FLOAT);
  }

  auto Ix = tier1::gaussian_derivative_func(device, src, nullptr, sigma, sigma, sigma, 1, 0, 0);
  auto Ixx = tier1::gaussian_derivative_func(device, Ix, nullptr, sigma, sigma, sigma, 1, 0, 0);
  auto Ixy = tier1::gaussian_derivative_func(device, Ix, nullptr, sigma, sigma, sigma, 0, 1, 0);

  const KernelInfo    kernel = { "hessian_gaussian_eigenvalues", kernel::hessian_gaussian_eigenvalues };
  const ParameterList params = { { "src", src },
                                  { "g_xx", Ixx },
                                  { "g_xy", Ixy },
                                 { "small_eigenvalue", small_eigenvalue },
                                 { "middle_eigenvalue", middle_eigenvalue },
                                 { "large_eigenvalue", large_eigenvalue } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  if (src->depth() == 1)
  {
    return { small_eigenvalue, large_eigenvalue };
  }
  return { small_eigenvalue, middle_eigenvalue, large_eigenvalue };
}


} // namespace cle::tier1
