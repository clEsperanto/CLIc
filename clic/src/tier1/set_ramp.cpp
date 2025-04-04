#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_set_ramp_x.h"
#include "cle_set_ramp_y.h"
#include "cle_set_ramp_z.h"

namespace cle::tier1
{

auto
set_ramp_x_func(const Device::Pointer & device, const Array::Pointer & src) -> Array::Pointer
{
  const KernelInfo    kernel_code = { "set_ramp_x", kernel::set_ramp_x };
  const ParameterList params = { { "dst", src } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel_code, params, range);
  return src;
}

auto
set_ramp_y_func(const Device::Pointer & device, const Array::Pointer & src) -> Array::Pointer
{
  const KernelInfo    kernel_code = { "set_ramp_y", kernel::set_ramp_y };
  const ParameterList params = { { "dst", src } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel_code, params, range);
  return src;
}

auto
set_ramp_z_func(const Device::Pointer & device, const Array::Pointer & src) -> Array::Pointer
{
  const KernelInfo    kernel_code = { "set_ramp_z", kernel::set_ramp_z };
  const ParameterList params = { { "dst", src } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel_code, params, range);
  return src;
}

} // namespace cle::tier1
