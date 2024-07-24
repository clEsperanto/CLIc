#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_local_cross_correlation.h"

namespace cle::tier1
{

auto
local_cross_correlation_func(const Device::Pointer & device,
                             const Array::Pointer &  src0,
                             const Array::Pointer &  src1,
                             Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src0, dst, dType::FLOAT);
  const KernelInfo    kernel = { "local_cross_correlation", kernel::local_cross_correlation };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
