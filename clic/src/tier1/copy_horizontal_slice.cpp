#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_copy_horizontal_slice_from.h"
#include "cle_copy_horizontal_slice_to.h"

namespace cle::tier1
{

auto
copy_horizontal_slice_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int slice_index)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  const ParameterList params = { { "src", src }, { "dst", dst }, { "index", slice_index } };
  KernelInfo          kernel;
  RangeArray          range;
  if (dst->depth() > 1)
  {
    kernel = { "copy_horizontal_slice_to", kernel::copy_horizontal_slice_to };
    range = { dst->width(), dst->height(), dst->depth() };
  }
  else
  {
    kernel = { "copy_horizontal_slice_from", kernel::copy_horizontal_slice_from };
    range = { dst->width(), dst->height(), dst->depth() };
  }
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
