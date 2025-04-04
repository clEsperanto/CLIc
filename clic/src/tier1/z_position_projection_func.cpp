#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_z_position_projection.h"

namespace cle::tier1
{

auto
z_position_projection_func(const Device::Pointer & device, const Array::Pointer & src,const Array::Pointer & position, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_xy(src, dst);
  const KernelInfo    kernel = { "z_position_projection", kernel::z_position_projection };
  const ParameterList params = { { "src", src }, { "position", position }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
