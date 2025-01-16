#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_minimum_of_masked_pixels_reduction.h"

namespace cle::tier1
{

auto
minimum_of_masked_pixels_reduction_func(const Device::Pointer & device,
                                        const Array::Pointer &  src,
                                        const Array::Pointer &  mask,
                                        Array::Pointer          reduced_src,
                                        Array::Pointer          reduced_mask) -> Array::Pointer
{
  tier0::create_xy(src, reduced_src);
  tier0::create_xy(mask, reduced_mask);
  const KernelInfo    kernel_code = { "minimum_of_masked_pixels_reduction", kernel::minimum_of_masked_pixels_reduction };
  const ParameterList params = {
    { "src", src }, { "mask", mask }, { "dst_src", reduced_src }, { "dst_mask", reduced_mask }
  };
  const RangeArray range = { reduced_src->width(), reduced_src->height(), reduced_src->depth() };
  execute(device, kernel_code, params, range);
  return reduced_src;
}

} // namespace cle::tier1
