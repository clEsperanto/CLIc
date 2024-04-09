#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_set_row.h"

namespace cle::tier1
{

auto
set_row_func(const Device::Pointer & device, const Array::Pointer & src, int row, float value) -> Array::Pointer
{
  const KernelInfo    kernel = { "set_row", kernel::set_row };
  const ParameterList params = { { "dst", src }, { "index", row }, { "scalar", value } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return src;
}

} // namespace cle::tier1
