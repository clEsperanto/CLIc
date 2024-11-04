#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_mode_box.h"
#include "cle_mode_sphere.h"

namespace cle::tier1
{

auto
mode_func(const Device::Pointer & device,
          const Array::Pointer &  src,
          Array::Pointer          dst,
          float                     radius_x,
          float                     radius_y,
          float                     radius_z,
          std::string             connectivity) -> Array::Pointer
{
  if (src->dtype() != dType::UINT8)
  {
    std::cerr << "Warning: mode only support uint8 pixel type." << std::endl;
  }
  tier0::create_like(src, dst, dType::UINT8);
  auto       r_x = radius2kernelsize(radius_x);
  auto       r_y = radius2kernelsize(radius_y);
  auto       r_z = radius2kernelsize(radius_z);
  KernelInfo kernel = { "mode_box", kernel::mode_box };
  if (connectivity == "sphere")
  {
    kernel = { "mode_sphere", kernel::mode_sphere };
  }
  const ParameterList params = {
    { "src", src }, { "dst", dst }, { "scalar0", r_x }, { "scalar1", r_y }, { "scalar2", r_z }
  };
  const RangeArray range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
mode_sphere_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 float                     radius_x,
                 float                     radius_y,
                 float                     radius_z) -> Array::Pointer
{
  return mode_func(device, src, dst, radius_x, radius_y, radius_z, "sphere");
}

auto
mode_box_func(const Device::Pointer & device,
              const Array::Pointer &  src,
              Array::Pointer          dst,
              float                     radius_x,
              float                     radius_y,
              float                     radius_z) -> Array::Pointer
{
  return mode_func(device, src, dst, radius_x, radius_y, radius_z, "box");
}

} // namespace cle::tier1
