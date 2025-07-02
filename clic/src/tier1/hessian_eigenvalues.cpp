#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_hessian_eigenvalues.h"

namespace cle::tier1
{

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
//   tier0::create_like(src, dst, dType::BINARY);
//   if (src->dtype() != dType::BINARY)
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
