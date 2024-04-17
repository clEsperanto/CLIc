#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_transpose_yz.h"

namespace cle::tier1
{

auto
transpose_yz_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  if (dst == nullptr)
  {
    auto dim = shape_to_dimension(src->width(), src->depth(), src->height());
    dst = Array::create(src->width(), src->depth(), src->height(), 3, src->dtype(), src->mtype(), src->device());
  }
  const KernelInfo    kernel = { "transpose_yz", kernel::transpose_yz };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
