#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"


#include "cle_maximum_projection.h"
#include "cle_mean_projection.h"
#include "cle_minimum_projection.h"
#include "cle_sum_projection.h"

#include "cle_x_position_of_maximum_x_projection.h"
#include "cle_x_position_of_minimum_x_projection.h"
#include "cle_y_position_of_maximum_y_projection.h"
#include "cle_y_position_of_minimum_y_projection.h"
#include "cle_z_position_of_maximum_z_projection.h"
#include "cle_z_position_of_minimum_z_projection.h"
#include "cle_z_position_projection.h"

namespace cle::tier1
{

auto
maximum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_zy(src, dst);
  const KernelInfo    kernel = { "maximum_projection", kernel::maximum_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local = { 1, 1, 1 };
  const ConstantList  constants = { { "PROJECTION_AXIS", 0 } }; // 0 for X axis
  execute(device, kernel, params, range, local, constants);
  return dst;
}

auto
maximum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xz(src, dst);
  const KernelInfo    kernel = { "maximum_projection", kernel::maximum_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local = { 1, 1, 1 };
  const ConstantList  constants = { { "PROJECTION_AXIS", 1 } }; // 1 for Y axis
  execute(device, kernel, params, range, local, constants);
  return dst;
}

auto
maximum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xy(src, dst);
  const KernelInfo    kernel = { "maximum_projection", kernel::maximum_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local = { 1, 1, 1 };
  const ConstantList  constants = { { "PROJECTION_AXIS", 2 } }; // 2 for Z axis
  execute(device, kernel, params, range, local, constants);
  return dst;
}

auto
mean_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_zy(src, dst);
  const KernelInfo    kernel = { "mean_projection", kernel::mean_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local = { 1, 1, 1 };
  const ConstantList  constants = { { "PROJECTION_AXIS", 0 } }; // 0 for X axis
  execute(device, kernel, params, range, local, constants);
  return dst;
}

auto
mean_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xz(src, dst);
  const KernelInfo    kernel = { "mean_projection", kernel::mean_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local = { 1, 1, 1 };
  const ConstantList  constants = { { "PROJECTION_AXIS", 1 } }; // 1 for Y axis
  execute(device, kernel, params, range, local, constants);
  return dst;
}

auto
mean_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xy(src, dst);
  const KernelInfo    kernel = { "mean_projection", kernel::mean_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local = { 1, 1, 1 };
  const ConstantList  constants = { { "PROJECTION_AXIS", 2 } }; // 2 for Z axis
  execute(device, kernel, params, range, local, constants);
  return dst;
}

auto
minimum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_zy(src, dst);
  const KernelInfo    kernel = { "minimum_projection", kernel::minimum_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local = { 1, 1, 1 };
  const ConstantList  constants = { { "PROJECTION_AXIS", 0 } }; // 0 for X axis
  execute(device, kernel, params, range, local, constants);
  return dst;
}
auto
minimum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xz(src, dst);
  const KernelInfo    kernel = { "minimum_projection", kernel::minimum_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local = { 1, 1, 1 };
  const ConstantList  constants = { { "PROJECTION_AXIS", 1 } }; // 1 for Y axis
  execute(device, kernel, params, range, local, constants);
  return dst;
}

auto
minimum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xy(src, dst);
  const KernelInfo    kernel = { "minimum_projection", kernel::minimum_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local = { 1, 1, 1 };
  const ConstantList  constants = { { "PROJECTION_AXIS", 2 } }; // 2 for Z axis
  execute(device, kernel, params, range, local, constants);
  return dst;
}

auto
sum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_zy(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "sum_projection", kernel::sum_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local = { 1, 1, 1 };
  const ConstantList  constants = { { "PROJECTION_AXIS", 0 } }; // 0 for X axis
  execute(device, kernel, params, range, local, constants);
  return dst;
}

auto
sum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xz(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "sum_projection", kernel::sum_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local = { 1, 1, 1 };
  const ConstantList  constants = { { "PROJECTION_AXIS", 1 } }; // 1 for Y axis
  execute(device, kernel, params, range, local, constants);
  return dst;
}

auto
sum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xy(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "sum_projection", kernel::sum_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const RangeArray    local = { 1, 1, 1 };
  const ConstantList  constants = { { "PROJECTION_AXIS", 2 } }; // 2 for Z axis
  execute(device, kernel, params, range, local, constants);
  return dst;
}

auto
x_position_of_maximum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_zy(src, dst, dType::INDEX);
  const KernelInfo    kernel = { "x_position_of_maximum_x_projection", kernel::x_position_of_maximum_x_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
x_position_of_minimum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_zy(src, dst, dType::INDEX);
  const KernelInfo    kernel = { "x_position_of_minimum_x_projection", kernel::x_position_of_minimum_x_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
y_position_of_maximum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xz(src, dst, dType::INDEX);
  const KernelInfo    kernel = { "y_position_of_maximum_y_projection", kernel::y_position_of_maximum_y_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
y_position_of_minimum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xz(src, dst, dType::INDEX);
  const KernelInfo    kernel = { "y_position_of_minimum_y_projection", kernel::y_position_of_minimum_y_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
z_position_of_maximum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xy(src, dst, dType::INDEX);
  const KernelInfo    kernel = { "z_position_of_maximum_z_projection", kernel::z_position_of_maximum_z_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
z_position_of_minimum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xy(src, dst, dType::INDEX);
  const KernelInfo    kernel = { "z_position_of_minimum_z_projection", kernel::z_position_of_minimum_z_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
z_position_projection_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & position, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_xy(src, dst);
  const KernelInfo    kernel = { "z_position_projection", kernel::z_position_projection };
  const ParameterList params = { { "src", src }, { "position", position }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
