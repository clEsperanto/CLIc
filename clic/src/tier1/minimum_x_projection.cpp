#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_minimum_x_projection.h"
#include "cle_x_position_of_minimum_x_projection.h"

namespace cle::tier1
{

auto
minimum_x_projection_func(const Device::Pointer & device,
                          const Array::Pointer &  src,
                          Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_zy(src, dst);
  const KernelInfo    kernel = { "minimum_x_projection", kernel::minimum_x_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
