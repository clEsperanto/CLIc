#include "tier0.hpp"
#include "tier1.hpp"
#include "utils.hpp"

#include "cle_gradient.h"
#include "cle_hessian_eigenvalues.h"
#include "cle_laplace_box.h"
#include "cle_laplace_diamond.h"
#include "cle_sobel.h"

namespace cle::tier1
{

auto
sobel_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "sobel", kernel::sobel };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
laplace_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, std::string connectivity) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  KernelInfo kernel = { "laplace_box", kernel::laplace_box };
  if (connectivity == "sphere")
  {
    kernel = { "laplace_diamond", kernel::laplace_diamond };
  }
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
laplace_box_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return laplace_func(device, src, dst, "box");
}

auto
laplace_diamond_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return laplace_func(device, src, dst, "sphere");
}

auto
gradient_x_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "gradient", kernel::gradient };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "axis", 0 } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
gradient_y_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "gradient", kernel::gradient };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "axis", 1 } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
gradient_z_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "gradient", kernel::gradient };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "axis", 2 } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
hessian_eigenvalues_func(const Device::Pointer & device,
                         const Array::Pointer &  src,
                         Array::Pointer          small_eigenvalue,
                         Array::Pointer          middle_eigenvalue,
                         Array::Pointer          large_eigenvalue) -> std::vector<Array::Pointer>
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
  const KernelInfo    kernel = { "hessian_eigenvalues", kernel::hessian_eigenvalues };
  const ParameterList params = { { "src", src },
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

// inferior_superior(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
// {
//   tier0::create_like(src, dst, dType::BOOL);
//   if (src->dtype() != dType::BOOL)
//   {
//     throw std::runtime_error("inferior_superior only supports BINARY (uint8) images");
//   }
//   const KernelInfo    kernel = { "inferior_superior", kernel::inferior_superior };
//   const ParameterList params = { { "src", src }, { "dst", dst } };
//   const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
//   execute(device, kernel, params, range);
//   return dst;
// }

} // namespace cle::tier1
