#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_mean_separable.h"
#include "cle_mean_sphere.h"
#include "cle_mean_x_projection.h"
#include "cle_mean_y_projection.h"
#include "cle_mean_z_projection.h"

namespace cle::tier1
{

auto
mean_func(const Device::Pointer & device,
          const Array::Pointer &  src,
          Array::Pointer          dst,
          int                     radius_x,
          int                     radius_y,
          int                     radius_z,
          std::string             connectivity) -> Array::Pointer
{
  tier0::create_like(src, dst);
  auto r_x = radius2kernelsize(radius_x);
  auto r_y = radius2kernelsize(radius_y);
  auto r_z = radius2kernelsize(radius_z);
  if (connectivity == "sphere")
  {
    const KernelInfo    kernel = { "mean_sphere", kernel::mean_sphere };
    const ParameterList params = {
      { "src", src }, { "dst", dst }, { "scalar0", r_x }, { "scalar1", r_y }, { "scalar2", r_z }
    };
    const RangeArray range = { dst->width(), dst->height(), dst->depth() };
    execute(device, kernel, params, range);
  }
  else
  {
    const KernelInfo kernel = { "mean_separable", kernel::mean_separable };
    tier0::execute_separable_func(
      device,
      kernel,
      src,
      dst,
      { static_cast<float>(radius_x), static_cast<float>(radius_y), static_cast<float>(radius_z) },
      { r_x, r_y, r_z });
  }
  return dst;
}

} // namespace cle::tier1
