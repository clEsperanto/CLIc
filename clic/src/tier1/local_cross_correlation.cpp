#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_local_cross_correlation.h"

namespace cle::tier1
{

auto
local_cross_correlation_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & kernel, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  const KernelInfo    oclkernel = { "local_cross_correlation", kernel::local_cross_correlation };
  const ParameterList params = { { "src0", src }, { "src1", kernel }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, oclkernel, params, range);
  return dst;
}

} // namespace cle::tier1
