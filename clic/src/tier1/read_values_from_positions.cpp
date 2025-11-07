#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_read_values_from_positions.h"

namespace cle::tier1
{

auto
read_values_from_positions_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & list, Array::Pointer dst)
  -> Array::Pointer
{
  // TODO: check list shape
  // if (list->height() < src->dim())
  // {
  //   throw std::runtime_error("The list height is expected to be " + std::to_string(src->dim()) + ", but it is " +
  //                            std::to_string(list->width()));
  // }
  tier0::create_vector(src, dst, list->width());
  const KernelInfo    kernel = { "read_values_from_positions", kernel::read_values_from_positions };
  const ParameterList params = { { "src0", src }, { "src1", list }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
