#include "tier0.hpp"
#include "tier1.hpp"
#include "utils.hpp"

#include "cle_mask.h"
#include "cle_mask_label.h"
#include "cle_minimum_of_masked_pixels_reduction.h"

namespace cle::tier1
{

auto
mask_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & mask, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_or_check_broadcast_dst(src, mask, dst, src->dtype());
  const KernelInfo    kernel = { "mask", kernel::mask };
  const ParameterList params = { { "src0", src }, { "src1", mask }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
mask_label_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst, float label)
  -> Array::Pointer
{
  tier0::create_or_check_broadcast_dst(src0, src1, dst, promoteType(src0->dtype(), src1->dtype()));

  const KernelInfo    kernel = { "mask_label", kernel::mask_label };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst }, { "scalar", label } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
