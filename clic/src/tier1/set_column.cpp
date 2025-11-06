#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_set_column.h"

namespace cle::tier1
{

auto
set_column_func(const Device::Pointer & device, const Array::Pointer & src, int column_index, float value) -> Array::Pointer
{
  const KernelInfo    kernel = { "set_column", kernel::set_column };
  const ParameterList params = { { "dst", src }, { "index", column_index }, { "scalar", value } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return src;
}

} // namespace cle::tier1
