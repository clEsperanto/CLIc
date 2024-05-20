#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_set_where_x_smaller_than_y.h"

namespace cle::tier1
{

auto
set_where_x_smaller_than_y_func(const Device::Pointer & device,
                                const Array::Pointer &  src,
                                float                   value) -> Array::Pointer
{
  const KernelInfo    kernel = { "set_where_x_smaller_than_y", kernel::set_where_x_smaller_than_y };
  const ParameterList params = { { "dst", src }, { "scalar", value } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return src;
}

} // namespace cle::tier1
