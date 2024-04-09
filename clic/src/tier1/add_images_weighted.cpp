#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_add_images_weighted.h"

namespace cle::tier1
{

auto
add_images_weighted_func(const Device::Pointer & device,
                         const Array::Pointer &  src0,
                         const Array::Pointer &  src1,
                         Array::Pointer          dst,
                         float                   factor0,
                         float                   factor1) -> Array::Pointer
{
  tier0::create_like(src0, dst, dType::FLOAT);
  const KernelInfo    kernel = { "add_images_weighted", kernel::add_images_weighted };
  const ParameterList params = {
    { "src0", src0 }, { "src1", src1 }, { "dst", dst }, { "scalar0", factor0 }, { "scalar1", factor1 }
  };
  const RangeArray range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
