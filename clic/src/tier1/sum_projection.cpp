#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_sum_x_projection.h"
#include "cle_sum_y_projection.h"
#include "cle_sum_z_projection.h"

namespace cle::tier1
{

auto
sum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_zy(src, dst, dType::FLOAT);
  const KernelInfo    kernel_code = { "sum_x_projection", kernel::sum_x_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel_code, params, range);
  return dst;
}

auto
sum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xz(src, dst, dType::FLOAT);
  const KernelInfo    kernel_code = { "sum_y_projection", kernel::sum_y_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel_code, params, range);
  return dst;
}

auto
sum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xy(src, dst, dType::FLOAT);
  const KernelInfo    kernel_code = { "sum_z_projection", kernel::sum_z_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel_code, params, range);
  return dst;
}

} // namespace cle::tier1
