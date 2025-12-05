#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_gradient.h"

namespace cle::tier1
{

auto
gradient_x_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "gradient", kernel::gradient };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "axis", 0 } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
gradient_y_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "gradient", kernel::gradient };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "axis", 1 } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
gradient_z_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "gradient", kernel::gradient };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "axis", 2 } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}


} // namespace cle::tier1
