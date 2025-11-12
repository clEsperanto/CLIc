#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_nan_to_num.h"

namespace cle::tier1
{

auto
nan_to_num_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float nan, float posinf, float neginf)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "nan_to_num", kernel::nan_to_num };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "nan", nan }, { "pinf", posinf }, { "ninf", neginf } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
