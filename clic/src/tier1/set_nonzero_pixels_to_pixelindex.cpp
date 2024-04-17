#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_set_nonzero_pixels_to_pixelindex.h"

namespace cle::tier1
{

auto
set_nonzero_pixels_to_pixelindex_func(const Device::Pointer & device,
                                      const Array::Pointer &  src,
                                      Array::Pointer          dst,
                                      int                     offset) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::INDEX);
  const KernelInfo    kernel = { "set_nonzero_pixels_to_pixelindex", kernel::set_nonzero_pixels_to_pixelindex };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "offset", offset } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
