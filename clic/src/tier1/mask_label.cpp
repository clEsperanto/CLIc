#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_mask_label.h"

namespace cle::tier1
{

auto
mask_label_func(const Device::Pointer & device,
                const Array::Pointer &  src,
                const Array::Pointer &  input_labels,
                Array::Pointer          dst,
                float                   label) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel_code = { "mask_label", kernel::mask_label };
  const ParameterList params = { { "src0", src }, { "src1", input_labels }, { "dst", dst }, { "scalar", label } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel_code, params, range);
  return dst;
}

} // namespace cle::tier1
