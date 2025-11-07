#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_minimum_y_projection.h"
#include "cle_y_position_of_minimum_y_projection.h"

namespace cle::tier1
{

auto
minimum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xz(src, dst);
  const KernelInfo    kernel = { "minimum_y_projection", kernel::minimum_y_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
