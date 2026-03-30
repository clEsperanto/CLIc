#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_copy_func.h"

namespace cle::tier1
{

auto
copy_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);

  if (src->dtype() == dst->dtype()) // use built-in copy if data types are the same
  {
    src->copyTo(dst);
    return dst;
  }

  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "copy", kernel::copy };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}


} // namespace cle::tier1
