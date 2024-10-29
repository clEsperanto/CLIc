#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_minimum_separable.h"
#include "cle_minimum_sphere.h"
namespace cle::tier1
{

auto
minimum_filter_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z,
                    std::string             connectivity) -> Array::Pointer
{
  tier0::create_like(src, dst);
  auto r_x = radius2kernelsize(radius_x);
  auto r_y = radius2kernelsize(radius_y);
  auto r_z = radius2kernelsize(radius_z);
  if (connectivity == "sphere")
  {
    const KernelInfo    kernel = { "minimum_sphere", kernel::minimum_sphere };
    const ParameterList params = {
      { "src", src }, { "dst", dst }, { "scalar0", r_x }, { "scalar1", r_y }, { "scalar2", r_z }
    };
    const RangeArray range = { dst->width(), dst->height(), dst->depth() };
    execute(device, kernel, params, range);
  }
  else
  {
    const KernelInfo kernel = { "minimum_separable", kernel::minimum_separable };
    execute_separable(device,
                      kernel,
                      src,
                      dst,
                      { static_cast<float>(radius_x), static_cast<float>(radius_y), static_cast<float>(radius_z) },
                      { r_x, r_y, r_z });
  }
  return dst;
}

auto
minimum_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z) -> Array::Pointer
{
  return minimum_filter_func(device, src, dst, radius_x, radius_y, radius_z, "sphere");
}

auto
minimum_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 int                     radius_x,
                 int                     radius_y,
                 int                     radius_z) -> Array::Pointer
{
  return minimum_filter_func(device, src, dst, radius_x, radius_y, radius_z, "box");
}

auto
grayscale_erode_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z,
                    std::string             connectivity) -> Array::Pointer
                    {
                      return minimum_filter_func(device, src, dst, radius_x, radius_y, radius_z, connectivity);
                    }

} // namespace cle::tier1
