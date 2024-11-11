#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_dilate_box.h"
#include "cle_dilate_sphere.h"
#include "cle_dilation.h"

namespace cle::tier1
{

auto
dilation_func(const Device::Pointer & device,
              const Array::Pointer &  src,
              const Array::Pointer &  footprint,
              Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  if (src->dimension() != footprint->dimension())
  {
    throw std::runtime_error(
      "Error: input and structuring element in dilation operator must have the same dimensionality.");
  }
  KernelInfo          kernel = { "dilation", kernel::dilation };
  const ParameterList params = { { "src", src }, { "footprint", footprint }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
binary_dilate_func(const Device::Pointer & device,
                   const Array::Pointer &  src,
                   Array::Pointer          dst,
                   float                   radius_x,
                   float                   radius_y,
                   float                   radius_z,
                   std::string             connectivity) -> Array::Pointer
{
  tier0::create_like(src, dst);
  auto       r_x = radius2kernelsize(radius_x);
  auto       r_y = radius2kernelsize(radius_y);
  auto       r_z = radius2kernelsize(radius_z);
  KernelInfo kernel = { "dilate_box", kernel::dilate_box };
  if (connectivity == "sphere")
  {
    kernel = { "dilate_sphere", kernel::dilate_sphere };
  }
  const ParameterList params = {
    { "src", src }, { "dst", dst }, { "scalar0", r_x }, { "scalar1", r_y }, { "scalar2", r_z }
  };
  const RangeArray range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
dilate_box_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return binary_dilate_func(device, src, dst, 1, 1, 1, "box");
}

auto
dilate_sphere_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return binary_dilate_func(device, src, dst, 1, 1, 1, "sphere");
}

auto
grayscale_dilate_func(const Device::Pointer & device,
                      const Array::Pointer &  src,
                      Array::Pointer          dst,
                      float                   radius_x,
                      float                   radius_y,
                      float                   radius_z,
                      std::string             connectivity) -> Array::Pointer
{
  return tier1::maximum_filter_func(device, src, dst, radius_x, radius_y, radius_z, connectivity);
}

} // namespace cle::tier1
