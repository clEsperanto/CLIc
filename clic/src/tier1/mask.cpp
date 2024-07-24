#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_mask.h"
#include "cle_mask_label.h"
#include "cle_minimum_of_masked_pixels_reduction.h"

namespace cle::tier1
{

auto
mask_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & mask, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "mask", kernel::mask };
  const ParameterList params = { { "src0", src }, { "src1", mask }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
