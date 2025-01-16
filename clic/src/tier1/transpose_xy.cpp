#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_transpose_xy.h"

namespace cle::tier1
{

auto
transpose_xy_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  if (dst == nullptr)
  {
    auto dim = shape_to_dimension(src->height(), src->width(), src->depth());
    dst = Array::create(src->height(), src->width(), src->depth(), dim, src->dtype(), src->mtype(), src->device());
  }
  const KernelInfo    kernel_code = { "transpose_xy", kernel::transpose_xy };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel_code, params, range);
  return dst;
}

} // namespace cle::tier1
