#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_gaussian_blur_separable.h"

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
  tier0::execute_separable_func(device,
                                kernel,
                                temp,
                                dst,
                                { sigma_x, sigma_y, sigma_z },
                                { sigma2kernelsize(sigma_x), sigma2kernelsize(sigma_y), sigma2kernelsize(sigma_z) });
  return dst;
}

// generate_angle_matrix_func
// generate_binary_overlap_matrix_func

} // namespace cle::tier1
