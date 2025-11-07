#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_circular_shift.h"

namespace cle::tier1
{

auto
circular_shift_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    const int               shift_x,
                    const int               shift_y,
                    const int               shift_z) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "circular_shift", kernel::circular_shift };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "index_x", shift_x }, { "index_y", shift_y }, { "index_z", shift_z } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
