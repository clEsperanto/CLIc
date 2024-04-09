#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_sign.h"

namespace cle::tier1
{

auto
sign_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  dst->fill(0);
  const KernelInfo    kernel = { "pixel_sign", kernel::sign };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
