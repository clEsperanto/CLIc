#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_median_box.h"
#include "cle_median_sphere.h"

namespace cle::tier1
{

auto
median_func(const Device::Pointer & device,
            const Array::Pointer &  src,
            Array::Pointer          dst,
            int                     radius_x,
            int                     radius_y,
            int                     radius_z,
            std::string             connectivity) -> Array::Pointer
{
  tier0::create_like(src, dst);
  auto       r_x = radius2kernelsize(radius_x);
  auto       r_y = radius2kernelsize(radius_y);
  auto       r_z = radius2kernelsize(radius_z);
  KernelInfo kernel = { "median_box", kernel::median_box };
  if (connectivity == "sphere")
  {
    kernel = { "median_sphere", kernel::median_sphere };
  }
  const ParameterList params = {
    { "src", src }, { "dst", dst }, { "scalar0", r_x }, { "scalar1", r_y }, { "scalar2", r_z }
  };
  const ConstantList constants = { { "MAX_ARRAY_SIZE", r_x * r_y * r_z } };
  const RangeArray   range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range, constants);
  return dst;
}

} // namespace cle::tier1
