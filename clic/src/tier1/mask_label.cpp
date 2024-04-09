#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_mask_label.h"

namespace cle::tier1
{

auto
mask_label_func(const Device::Pointer & device,
                const Array::Pointer &  src0,
                const Array::Pointer &  src1,
                Array::Pointer          dst,
                float                   label) -> Array::Pointer
{
  tier0::create_like(src0, dst);
  const KernelInfo    kernel = { "mask_label", kernel::mask_label };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst }, { "scalar", label } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
