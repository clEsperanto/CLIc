#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_set_image_borders.h"

namespace cle::tier1
{

auto
set_image_borders_func(const Device::Pointer & device, const Array::Pointer & src, float value) -> Array::Pointer
{
  const KernelInfo    kernel = { "set_image_borders", kernel::set_image_borders };
  const ParameterList params = { { "src", src }, { "scalar", value } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return src;
}

} // namespace cle::tier1
