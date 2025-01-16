#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_transpose_xz.h"

namespace cle::tier1
{

auto
transpose_xz_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  if (dst == nullptr)
  {
    auto dim = shape_to_dimension(src->depth(), src->height(), src->width());
    dst = Array::create(src->depth(), src->height(), src->width(), dim, src->dtype(), src->mtype(), src->device());
  }
  const KernelInfo    kernel_code = { "transpose_xz", kernel::transpose_xz };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel_code, params, range);
  return dst;
}

} // namespace cle::tier1
