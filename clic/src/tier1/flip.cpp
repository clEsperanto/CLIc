#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_flip.h"

namespace cle::tier1
{

auto
flip_func(const Device::Pointer & device,
          const Array::Pointer &  src,
          Array::Pointer          dst,
          bool                    flip_x,
          bool                    flip_y,
          bool                    flip_z) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "flip", kernel::flip };
  const ParameterList params = {
    { "src", src }, { "dst", dst }, { "index0", int(flip_x) }, { "index1", int(flip_y) }, { "index2", int(flip_z) }
  };
  const RangeArray range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
