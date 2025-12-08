#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_transpose.h"

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
  const KernelInfo    kernel = { "transpose", kernel::transpose };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local = { 1, 1, 1 };
  const ConstantList  constants = { { "TRANSPOSE_MODE", "XY" } };
  execute(device, kernel, params, range, local, constants);
  return dst;
}

auto
transpose_xz_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  if (dst == nullptr)
  {
    auto dim = shape_to_dimension(src->depth(), src->height(), src->width());
    dst = Array::create(src->depth(), src->height(), src->width(), dim, src->dtype(), src->mtype(), src->device());
  }
  const KernelInfo    kernel = { "transpose", kernel::transpose };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local = { 1, 1, 1 };
  const ConstantList  constants = { { "TRANSPOSE_MODE", "XZ" } };
  execute(device, kernel, params, range, local, constants);
  return dst;
}

auto
transpose_yz_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  if (dst == nullptr)
  {
    auto dim = shape_to_dimension(src->width(), src->depth(), src->height());
    dst = Array::create(src->width(), src->depth(), src->height(), 3, src->dtype(), src->mtype(), src->device());
  }
  const KernelInfo    kernel = { "transpose", kernel::transpose };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local = { 1, 1, 1 };
  const ConstantList  constants = { { "TRANSPOSE_MODE", "YZ" } };
  execute(device, kernel, params, range, local, constants);
  return dst;
}


} // namespace cle::tier1
