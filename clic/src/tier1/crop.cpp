#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_crop.h"

namespace cle::tier1
{

auto
crop_func(const Device::Pointer & device,
          const Array::Pointer &  src,
          Array::Pointer          dst,
          int                     start_x,
          int                     start_y,
          int                     start_z,
          int                     width,
          int                     height,
          int                     depth) -> Array::Pointer
{
  tier0::create_dst(src, dst, width, height, depth);
  const KernelInfo    kernel_code = { "crop", kernel::crop };
  const ParameterList params = {
    { "src", src }, { "dst", dst }, { "index0", start_x }, { "index1", start_y }, { "index2", start_z }
  };
  const RangeArray range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel_code, params, range);
  return dst;
}

} // namespace cle::tier1
