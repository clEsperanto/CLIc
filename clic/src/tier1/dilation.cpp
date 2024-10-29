#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_dilate_box.h"
#include "cle_dilate_sphere.h"
#include "cle_dilation.h"

namespace cle::tier1
{

auto
dilation_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & strel, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  if( src->dimension() != strel->dimension() )
  {
    throw std::runtime_error("Error: input and structuring element in dilation operator must have the same dimensionality.");
  }
  KernelInfo kernel = { "dilation", kernel::dilation };
  const ParameterList params = { { "src", src }, { "strel", strel }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}   

auto
binary_dilate_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, std::string connectivity)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  KernelInfo kernel = { "dilate_box", kernel::dilate_box };
  if (connectivity == "sphere")
  {
    kernel = { "dilate_sphere", kernel::dilate_sphere };
  }
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
dilate_box_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return binary_dilate_func(device, src, dst, "box");
}

auto
dilate_sphere_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return binary_dilate_func(device, src, dst, "sphere");
}

} // namespace cle::tier1
