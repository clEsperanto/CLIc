#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_copy_vertical_slice_from.h"
#include "cle_copy_vertical_slice_to.h"

namespace cle::tier1
{

auto
copy_vertical_slice_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int slice)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  const ParameterList params = { { "src", src }, { "dst", dst }, { "index", slice } };
  KernelInfo          kernel;
  RangeArray          range;
  if (dst->depth() > 1)
  {
    kernel = { "copy_vertical_slice_to", kernel::copy_vertical_slice_to };
    range = { src->width(), src->height(), 1 };
  }
  else
  {
    kernel = { "copy_vertical_slice_from", kernel::copy_vertical_slice_from };
    range = { dst->width(), dst->height(), dst->depth() };
  }
  execute(device, kernel, params, range);
  return dst;
}

// count_touching_neighbors_func

} // namespace cle::tier1
