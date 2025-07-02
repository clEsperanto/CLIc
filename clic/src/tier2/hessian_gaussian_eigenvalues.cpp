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

  // scale sigma (https://github.com/scikit-image/scikit-image/blob/be7ff3442864f8c44236c4cd50c04039a85b3ea8/skimage/feature/corner.py#L196)
  constexpr float one = 1.0F; // std::sqrt(2.0F);
  constexpr int truncate = 8;
  const float sq1_2 = 1.0F / std::sqrt(2.0F);
  sigma *= sq1_2;

  size_t radius = static_cast<size_t>(truncate * sigma + 0.5F);
  size_t kernel_size = static_cast<size_t>(2 * radius + 1);
  Array::Pointer dirac = Array::create( ( src->width()>1) ? kernel_size : 1,
                                        ( src->height()>1) ? kernel_size : 1,
                                        ( src->depth()>1) ? kernel_size : 1,
                                        src->dimension(),
                                        dType::FLOAT,
                                        mType::BUFFER,
                                        device);
  dirac->fill(0.0F);
  dirac->writeFrom(&one, {1,1,1}, {static_cast<size_t>(dirac->width()/2), dirac->height()/2, dirac->depth()/2});

  // compute the Gaussian first derivatives along x and blur along y to get Gx
  auto Gx = tier1::gaussian_derivative_func(device, dirac, nullptr, sigma, sigma, sigma, 1, 0, 0);
  // compute the Gaussian second derivatives along x and blur along y to get Gxx
  // compute the Gaussian second derivatives along y and blur along x to get Gxy
  auto Gxx = tier1::gaussian_derivative_func(device, Gx, nullptr, sigma, sigma, sigma, 1, 0, 0);
  auto Gxy = tier1::gaussian_derivative_func(device, Gx, nullptr, sigma, sigma, sigma, 0, 1, 0);

  const KernelInfo    kernel = { "hessian_gaussian_eigenvalues", kernel::hessian_gaussian_eigenvalues };
  const ParameterList params = { { "src", src },
                                  { "g_xx", Gxx },
                                  { "g_xy", Gxy },
                                 { "small_eigenvalue", small_eigenvalue },
                                 { "middle_eigenvalue", middle_eigenvalue },
                                 { "large_eigenvalue", large_eigenvalue } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  if (src->depth() == 1)
  {
    return { large_eigenvalue, small_eigenvalue };
  }
  return { large_eigenvalue, middle_eigenvalue, small_eigenvalue };
}


} // namespace cle::tier1

