#include "tier0.hpp"
#include "tier1.hpp"
#include "utils.hpp"

#include "cle_maximum_separable.h"
#include "cle_maximum_sphere.h"
#include "cle_mean_separable.h"
#include "cle_mean_sphere.h"
#include "cle_median_box.h"
#include "cle_median_sphere.h"
#include "cle_minimum_separable.h"
#include "cle_minimum_sphere.h"
#include "cle_mode_box.h"
#include "cle_mode_sphere.h"
#include "cle_variance_box.h"
#include "cle_variance_sphere.h"

namespace cle::tier1
{

auto
maximum_filter_func(const Device::Pointer & device,
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
    const KernelInfo    kernel = { "maximum_sphere", kernel::maximum_sphere };
    const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar0", r_x }, { "scalar1", r_y }, { "scalar2", r_z } };
    const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
    execute(device, kernel, params, range);
  }
  else
  {
    const KernelInfo kernel = { "maximum_separable", kernel::maximum_separable };
    execute_separable(device,
                      kernel,
                      src,
                      dst,
                      { static_cast<float>(radius_x), static_cast<float>(radius_y), static_cast<float>(radius_z) },
                      { r_x, r_y, r_z },
                      { 0, 0, 0 });
  }
  return dst;
}

auto
maximum_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z) -> Array::Pointer
{
  return maximum_filter_func(device, src, dst, radius_x, radius_y, radius_z, "sphere");
}

auto
maximum_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 float                   radius_x,
                 float                   radius_y,
                 float                   radius_z) -> Array::Pointer
{
  return maximum_filter_func(device, src, dst, radius_x, radius_y, radius_z, "box");
}

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
    const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar0", r_x }, { "scalar1", r_y }, { "scalar2", r_z } };
    const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
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
                      { r_x, r_y, r_z },
                      { 0, 0, 0 });
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
                 float                   radius_x,
                 float                   radius_y,
                 float                   radius_z) -> Array::Pointer
{
  return minimum_filter_func(device, src, dst, radius_x, radius_y, radius_z, "box");
}

auto
mean_filter_func(const Device::Pointer & device,
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
    const KernelInfo    kernel = { "mean_sphere", kernel::mean_sphere };
    const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar0", r_x }, { "scalar1", r_y }, { "scalar2", r_z } };
    const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
    execute(device, kernel, params, range);
  }
  else
  {
    const KernelInfo kernel = { "mean_separable", kernel::mean_separable };
    execute_separable(device,
                      kernel,
                      src,
                      dst,
                      { static_cast<float>(radius_x), static_cast<float>(radius_y), static_cast<float>(radius_z) },
                      { r_x, r_y, r_z },
                      { 0, 0, 0 });
  }
  return dst;
}

auto
mean_sphere_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 float                   radius_x,
                 float                   radius_y,
                 float                   radius_z) -> Array::Pointer
{
  return mean_filter_func(device, src, dst, radius_x, radius_y, radius_z, "sphere");
}

auto
mean_box_func(const Device::Pointer & device,
              const Array::Pointer &  src,
              Array::Pointer          dst,
              float                   radius_x,
              float                   radius_y,
              float                   radius_z) -> Array::Pointer
{
  return mean_filter_func(device, src, dst, radius_x, radius_y, radius_z, "box");
}

auto
median_func(const Device::Pointer & device,
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

  auto median_size = r_x * r_y * r_z;
  if (median_size * src->itemSize() > device->getLocalMemorySize())
  {
    std::cerr << "Warning: The kernel size is too large for the device local memory. Total kernel size is " << median_size * src->itemSize()
              << " bytes, but the device local memory size is " << device->getLocalMemorySize() << " bytes." << std::endl;
  }

  KernelInfo kernel = { "median_box", kernel::median_box };
  if (connectivity == "sphere")
  {
    kernel = { "median_sphere", kernel::median_sphere };
  }
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar0", r_x }, { "scalar1", r_y }, { "scalar2", r_z } };
  const ConstantList  constants = { { "MAX_ARRAY_SIZE", r_x * r_y * r_z } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local_range = { 0, 0, 0 }; // default
  execute(device, kernel, params, range, local_range, constants);
  return dst;
}

auto
median_box_func(const Device::Pointer & device,
                const Array::Pointer &  src,
                Array::Pointer          dst,
                float                   radius_x,
                float                   radius_y,
                float                   radius_z) -> Array::Pointer
{
  return median_func(device, src, dst, radius_x, radius_y, radius_z, "box");
}

auto
median_sphere_func(const Device::Pointer & device,
                   const Array::Pointer &  src,
                   Array::Pointer          dst,
                   float                   radius_x,
                   float                   radius_y,
                   float                   radius_z) -> Array::Pointer
{
  return median_func(device, src, dst, radius_x, radius_y, radius_z, "sphere");
}

auto
mode_func(const Device::Pointer & device,
          const Array::Pointer &  src,
          Array::Pointer          dst,
          float                   radius_x,
          float                   radius_y,
          float                   radius_z,
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
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar0", r_x }, { "scalar1", r_y }, { "scalar2", r_z } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
mode_sphere_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 float                   radius_x,
                 float                   radius_y,
                 float                   radius_z) -> Array::Pointer
{
  return mode_func(device, src, dst, radius_x, radius_y, radius_z, "sphere");
}

auto
mode_box_func(const Device::Pointer & device,
              const Array::Pointer &  src,
              Array::Pointer          dst,
              float                   radius_x,
              float                   radius_y,
              float                   radius_z) -> Array::Pointer
{
  return mode_func(device, src, dst, radius_x, radius_y, radius_z, "box");
}

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
  KernelInfo kernel = { "variance_box", kernel::variance_box };
  if (connectivity == "sphere")
  {
    kernel = { "variance_sphere", kernel::variance_sphere };
  }
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar0", r_x }, { "scalar1", r_y }, { "scalar2", r_z } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
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
