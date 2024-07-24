#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_subtract_image_from_scalar.h"

namespace cle::tier1
{

auto
subtract_image_from_scalar_func(const Device::Pointer & device,
                                const Array::Pointer &  src,
                                Array::Pointer          dst,
                                float                   scalar) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "subtract_image_from_scalar", kernel::subtract_image_from_scalar };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar", scalar } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
