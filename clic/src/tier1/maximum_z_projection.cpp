#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_maximum_z_projection.h"
#include "cle_z_position_of_maximum_z_projection.h"

namespace cle::tier1
{

auto
maximum_z_projection_func(const Device::Pointer & device,
                          const Array::Pointer &  src,
                          Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_xy(src, dst);
  const KernelInfo    kernel = { "maximum_z_projection", kernel::maximum_z_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
