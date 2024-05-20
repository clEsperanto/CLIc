#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_x_position_of_maximum_x_projection.h"

namespace cle::tier1
{

auto
x_position_of_maximum_x_projection_func(const Device::Pointer & device,
                                        const Array::Pointer &  src,
                                        Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_zy(src, dst, dType::INDEX);
  const KernelInfo    kernel = { "x_position_of_maximum_x_projection", kernel::x_position_of_maximum_x_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
