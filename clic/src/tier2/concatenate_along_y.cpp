#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

namespace cle::tier2
{

auto
concatenate_along_y_func(const Device::Pointer & device,
                         const Array::Pointer &  src0,
                         const Array::Pointer &  src1,
                         Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_dst(src0, dst, src0->width(), src0->height() + src1->height(), src0->depth(), src0->dtype());
  tier1::paste_func(device, src0, dst, 0, 0, 0);
  tier1::paste_func(device, src1, dst, 0, src0->height(), 0);
  return dst;
}

} // namespace cle::tier2
