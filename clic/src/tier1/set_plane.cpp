#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_set_plane.h"

namespace cle::tier1
{

auto
set_plane_func(const Device::Pointer & device, const Array::Pointer & src, int plane_index, float value)
  -> Array::Pointer
{
  const KernelInfo    kernel_code = { "set_plane", kernel::set_plane };
  const ParameterList params = { { "dst", src }, { "index", plane_index }, { "scalar", value } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel_code, params, range);
  return src;
}

} // namespace cle::tier1
