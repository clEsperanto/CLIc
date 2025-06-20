#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_divide_images.h"

namespace cle::tier1
{

auto
divide_images_func(const Device::Pointer & device,
                   const Array::Pointer &  dividend,
                   const Array::Pointer &  divisor,
                   Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(dividend, dst);
  const KernelInfo    kernel_code = { "divide_images", kernel::divide_images };
  const ParameterList params = { { "src0", dividend }, { "src1", divisor }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel_code, params, range);
  return dst;
}

} // namespace cle::tier1
