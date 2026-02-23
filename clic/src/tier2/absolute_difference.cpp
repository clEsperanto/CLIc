#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

namespace cle::tier2
{

auto
absolute_difference_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_like(src0, dst);
  evaluate(device, "fabs(src0 - src1)", {src0, src1}, dst);
  return dst;
}

} // namespace cle::tier2
