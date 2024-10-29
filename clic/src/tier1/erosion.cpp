#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_erode_box.h"
#include "cle_erode_sphere.h"
#include "cle_erosion.h"

namespace cle::tier1
{

auto
erosion_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & strel, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  if( src->dimension() != strel->dimension() )
  {
    throw std::runtime_error("Error: input and structuring element in erosion operator must have the same dimensionality.");
  }
  KernelInfo kernel = { "erosion", kernel::erosion };
  const ParameterList params = { { "src", src }, { "strel", strel }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}    

auto
binary_erode_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, std::string connectivity)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  KernelInfo kernel = { "erode_box", kernel::erode_box };
  if (connectivity == "sphere")
  {
    kernel = { "erode_sphere", kernel::erode_sphere };
  }
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
erode_sphere_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return binary_erode_func(device, src, dst, "sphere");
}

auto
erode_box_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return binary_erode_func(device, src, dst, "box");
}

// erode_sphere_slice_by_slice_func

} // namespace cle::tier1