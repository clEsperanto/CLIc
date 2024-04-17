#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_smaller_or_equal.h"
#include "cle_smaller_or_equal_constant.h"

namespace cle::tier1
{

auto
smaller_or_equal_func(const Device::Pointer & device,
                      const Array::Pointer &  src0,
                      const Array::Pointer &  src1,
                      Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src0, dst, dType::BINARY);
  const KernelInfo    kernel = { "smaller_or_equal", kernel::smaller_or_equal };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
