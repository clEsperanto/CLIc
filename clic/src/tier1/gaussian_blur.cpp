#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_gaussian_blur_separable.h"
#include "cle_gaussian_derivative_separable.h"

namespace cle::tier1
{

auto
gaussian_blur_func(const Device::Pointer & device,
                   const Array::Pointer &  src,
                   Array::Pointer          dst,
                   float                   sigma_x,
                   float                   sigma_y,
                   float                   sigma_z) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  Array::Pointer temp = src;
  if (temp->dtype() != dType::FLOAT)
  {
    temp = Array::create(dst);
    tier1::copy_func(device, src, temp);
  }
  const KernelInfo kernel = { "gaussian_blur_separable", kernel::gaussian_blur_separable };
  execute_separable(device,
                    kernel,
                    temp,
                    dst,
                    { sigma_x, sigma_y, sigma_z },
                    { sigma2kernelsize(sigma_x), sigma2kernelsize(sigma_y), sigma2kernelsize(sigma_z) },
                    { 0, 0, 0 });
  return dst;
}

auto
gaussian_derivative_func(const Device::Pointer & device,
                         const Array::Pointer &  src,
                         Array::Pointer          dst,
                         float                   sigma_x,
                         float                   sigma_y,
                         float                   sigma_z,
                         int                     order_x,
                         int                     order_y,
                         int                     order_z) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  Array::Pointer temp = src;
  if (temp->dtype() != dType::FLOAT)
  {
    temp = Array::create(dst);
    tier1::copy_func(device, src, temp);
  }

  constexpr int        truncate = 8;
  std::array<float, 3> sigmas = { std::max({ sigma_x, 0.0f }), std::max({ sigma_y, 0.0f }), std::max({ sigma_z, 0.0f }) };
  std::array<int, 3>   radii = { static_cast<int>(truncate * sigmas[0] + 0.5f),
                                 static_cast<int>(truncate * sigmas[1] + 0.5f),
                                 static_cast<int>(truncate * sigmas[2] + 0.5f) };
  std::array<int, 3>   orders = { std::min(order_x, 2), std::min(order_y, 2), std::min(order_z, 2) };

  const KernelInfo kernel = { "gaussian_derivative_separable", kernel::gaussian_derivative_separable };
  execute_separable(device, kernel, temp, dst, sigmas, radii, orders);
  return dst;
}

} // namespace cle::tier1
