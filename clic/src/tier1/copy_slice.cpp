#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_copy_slice_from.h"
#include "cle_copy_slice_to.h"

#include "cle_copy_horizontal_slice_from.h"
#include "cle_copy_horizontal_slice_to.h"

#include "cle_copy_vertical_slice_from.h"
#include "cle_copy_vertical_slice_to.h"

namespace cle::tier1
{

auto
copy_slice_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int slice_index)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  const ParameterList params = { { "src", src }, { "dst", dst }, { "index", slice_index } };
  KernelInfo          kernel_code;
  RangeArray          range;
  if (dst->depth() > 1)
  {
    kernel_code = { "copy_slice_to", kernel::copy_slice_to };
    range = { src->width(), src->height(), 1 };
  }
  else
  {
    kernel_code = { "copy_slice_from", kernel::copy_slice_from };
    range = { dst->width(), dst->height(), dst->depth() };
  }
  execute(device, kernel_code, params, range);
  return dst;
}

auto
copy_horizontal_slice_func(const Device::Pointer & device,
                           const Array::Pointer &  src,
                           Array::Pointer          dst,
                           int                     slice_index) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const ParameterList params = { { "src", src }, { "dst", dst }, { "index", slice_index } };
  KernelInfo          kernel_code;
  RangeArray          range;
  if (dst->depth() > 1)
  {
    kernel_code = { "copy_horizontal_slice_to", kernel::copy_horizontal_slice_to };
    range = { dst->width(), dst->height(), dst->depth() };
  }
  else
  {
    kernel_code = { "copy_horizontal_slice_from", kernel::copy_horizontal_slice_from };
    range = { dst->width(), dst->height(), dst->depth() };
  }
  execute(device, kernel_code, params, range);
  return dst;
}

auto
copy_vertical_slice_func(const Device::Pointer & device,
                         const Array::Pointer &  src,
                         Array::Pointer          dst,
                         int                     slice_index) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const ParameterList params = { { "src", src }, { "dst", dst }, { "index", slice_index } };
  KernelInfo          kernel_code;
  RangeArray          range;
  if (dst->depth() > 1)
  {
    kernel_code = { "copy_vertical_slice_to", kernel::copy_vertical_slice_to };
    range = { src->width(), src->height(), 1 };
  }
  else
  {
    kernel_code = { "copy_vertical_slice_from", kernel::copy_vertical_slice_from };
    range = { dst->width(), dst->height(), dst->depth() };
  }
  execute(device, kernel_code, params, range);
  return dst;
}

} // namespace cle::tier1
