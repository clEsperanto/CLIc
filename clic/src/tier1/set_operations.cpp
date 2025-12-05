#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_set.h"
#include "cle_set_image_borders.h"
#include "cle_set_nonzero_pixels_to_pixelindex.h"

#include "cle_set_slice.h"
#include "cle_set_ramp.h"
#include "cle_set_where_x_compare_y.h"

namespace cle::tier1
{

auto
set_row_func(const Device::Pointer & device, const Array::Pointer & src, int row_index, float value) -> Array::Pointer
{
  const KernelInfo    kernel = { "set_slice", kernel::set_slice };
  const ParameterList params = { { "dst", src }, { "dimension", 0 }, { "index", row_index }, { "scalar", value } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return src;
}

auto
set_column_func(const Device::Pointer & device, const Array::Pointer & src, int column_index, float value) -> Array::Pointer
{
  const KernelInfo    kernel = { "set_slice", kernel::set_slice };
  const ParameterList params = { { "dst", src }, { "dimension", 1 }, { "index", column_index }, { "scalar", value } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return src;
}

auto
set_plane_func(const Device::Pointer & device, const Array::Pointer & src, int plane_index, float value) -> Array::Pointer
{
  const KernelInfo    kernel = { "set_slice", kernel::set_slice };
  const ParameterList params = { { "dst", src }, { "dimension", 2 }, { "index", plane_index }, { "scalar", value } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return src;
}

auto
set_ramp_x_func(const Device::Pointer & device, const Array::Pointer & src) -> Array::Pointer
{
  const KernelInfo    kernel = { "set_ramp", kernel::set_ramp };
  const ParameterList params = { { "dst", src }, { "dimension", 0 } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return src;
}

auto
set_ramp_y_func(const Device::Pointer & device, const Array::Pointer & src) -> Array::Pointer
{
  const KernelInfo    kernel = { "set_ramp", kernel::set_ramp };
  const ParameterList params = { { "dst", src }, { "dimension", 1 } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return src;
}

auto
set_ramp_z_func(const Device::Pointer & device, const Array::Pointer & src) -> Array::Pointer
{
  const KernelInfo    kernel = { "set_ramp", kernel::set_ramp };
  const ParameterList params = { { "dst", src } , { "dimension", 2 } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return src;
}

auto
set_where_x_equals_y_func(const Device::Pointer & device, const Array::Pointer & src, float value) -> Array::Pointer
{
  const KernelInfo    kernel = { "set_where_x_compare_y", kernel::set_where_x_compare_y };
  const ParameterList params = { { "dst", src }, { "scalar", value } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  const RangeArray    local = { 1 , 1 , 1 };
  const ConstantList  constants = { { "COMPARISON_OP(x,y)" , "(x == y)" } };
  execute(device, kernel, params, range, local, constants);
  return src;
}

auto
set_where_x_greater_than_y_func(const Device::Pointer & device, const Array::Pointer & src, float value) -> Array::Pointer
{
  const KernelInfo    kernel = { "set_where_x_compare_y", kernel::set_where_x_compare_y };
  const ParameterList params = { { "dst", src }, { "scalar", value } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  const RangeArray    local = { 1 , 1 , 1 };
  const ConstantList  constants = { { "COMPARISON_OP(x,y)" , "(x > y)" } };
  execute(device, kernel, params, range, local, constants);
  return src;
}

auto
set_where_x_smaller_than_y_func(const Device::Pointer & device, const Array::Pointer & src, float value) -> Array::Pointer
{
  const KernelInfo    kernel = { "set_where_x_compare_y", kernel::set_where_x_compare_y };
  const ParameterList params = { { "dst", src }, { "scalar", value } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  const RangeArray    local = { 1 , 1 , 1 };
  const ConstantList  constants = { { "COMPARISON_OP(x,y)" , "(x < y)" } };
  execute(device, kernel, params, range, local, constants);
  return src;
}

auto
set_nonzero_pixels_to_pixelindex_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int offset)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::INDEX);
  const KernelInfo    kernel = { "set_nonzero_pixels_to_pixelindex", kernel::set_nonzero_pixels_to_pixelindex };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "offset", offset } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
set_func(const Device::Pointer & device, const Array::Pointer & src, float scalar) -> Array::Pointer
{
  const KernelInfo    kernel = { "set", kernel::set };
  const ParameterList params = { { "dst", src }, { "scalar", scalar } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return src;
}

auto
set_image_borders_func(const Device::Pointer & device, const Array::Pointer & src, float value) -> Array::Pointer
{
  const KernelInfo    kernel = { "set_image_borders", kernel::set_image_borders };
  const ParameterList params = { { "src", src }, { "scalar", value } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return src;
}


} // namespace cle::tier1
