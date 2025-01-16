#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_variance_box.h"
#include "cle_variance_sphere.h"

namespace cle::tier1
{

auto
variance_filter_func(const Device::Pointer & device,
                     const Array::Pointer &  src,
                     Array::Pointer          dst,
                     float                   radius_x,
                     float                   radius_y,
                     float                   radius_z,
                     std::string             connectivity) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  auto       r_x = radius2kernelsize(radius_x);
  auto       r_y = radius2kernelsize(radius_y);
  auto       r_z = radius2kernelsize(radius_z);
  KernelInfo kernel_code = { "variance_box", kernel::variance_box };
  if (connectivity == "sphere")
  {
    kernel_code = { "variance_sphere", kernel::variance_sphere };
  }
  const ParameterList params = {
    { "src", src }, { "dst", dst }, { "scalar0", r_x }, { "scalar1", r_y }, { "scalar2", r_z }
  };
  const RangeArray range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel_code, params, range);
  return dst;
}

auto
variance_sphere_func(const Device::Pointer & device,
                     const Array::Pointer &  src,
                     Array::Pointer          dst,
                     float                   radius_x,
                     float                   radius_y,
                     float                   radius_z) -> Array::Pointer
{
  return variance_filter_func(device, src, dst, radius_x, radius_y, radius_z, "sphere");
}


auto
variance_box_func(const Device::Pointer & device,
                  const Array::Pointer &  src,
                  Array::Pointer          dst,
                  float                   radius_x,
                  float                   radius_y,
                  float                   radius_z) -> Array::Pointer
{
  return variance_filter_func(device, src, dst, radius_x, radius_y, radius_z, "box");
}

} // namespace cle::tier1
