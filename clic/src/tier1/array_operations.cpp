#include "tier0.hpp"
#include "tier1.hpp"
#include "utils.hpp"

#include "cle_circular_shift.h"
#include "cle_copy.h"
#include "cle_copy_horizontal_slice_from.h"
#include "cle_copy_horizontal_slice_to.h"
#include "cle_copy_slice_from.h"
#include "cle_copy_slice_to.h"
#include "cle_copy_vertical_slice_from.h"
#include "cle_copy_vertical_slice_to.h"
#include "cle_crop.h"
#include "cle_flip.h"
#include "cle_paste.h"
#include "cle_transpose.h"

#include <cmath>

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

auto
copy_slice_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int slice_index) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const ParameterList params = { { "src", src }, { "dst", dst }, { "index", slice_index } };
  KernelInfo          kernel;
  RangeArray          range;
  if (dst->depth() > 1)
  {
    kernel = { "copy_slice_to", kernel::copy_slice_to };
    range = { src->width(), src->height(), 1 };
  }
  else
  {
    kernel = { "copy_slice_from", kernel::copy_slice_from };
    range = { dst->width(), dst->height(), dst->depth() };
  }
  execute(device, kernel, params, range);
  return dst;
}

auto
copy_horizontal_slice_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int slice_index)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  const ParameterList params = { { "src", src }, { "dst", dst }, { "index", slice_index } };
  KernelInfo          kernel;
  RangeArray          range;
  if (dst->depth() > 1)
  {
    kernel = { "copy_horizontal_slice_to", kernel::copy_horizontal_slice_to };
    range = { dst->width(), dst->height(), dst->depth() };
  }
  else
  {
    kernel = { "copy_horizontal_slice_from", kernel::copy_horizontal_slice_from };
    range = { dst->width(), dst->height(), dst->depth() };
  }
  execute(device, kernel, params, range);
  return dst;
}

auto
copy_vertical_slice_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int slice_index) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const ParameterList params = { { "src", src }, { "dst", dst }, { "index", slice_index } };
  KernelInfo          kernel;
  RangeArray          range;
  if (dst->depth() > 1)
  {
    kernel = { "copy_vertical_slice_to", kernel::copy_vertical_slice_to };
    range = { src->width(), src->height(), 1 };
  }
  else
  {
    kernel = { "copy_vertical_slice_from", kernel::copy_vertical_slice_from };
    range = { dst->width(), dst->height(), dst->depth() };
  }
  execute(device, kernel, params, range);
  return dst;
}

auto
crop_func(const Device::Pointer & device,
          const Array::Pointer &  src,
          Array::Pointer          dst,
          int                     start_x,
          int                     start_y,
          int                     start_z,
          int                     width,
          int                     height,
          int                     depth) -> Array::Pointer
{
  if (dst != nullptr)
  {
    width = static_cast<int>(dst->width());
    height = static_cast<int>(dst->height());
    depth = static_cast<int>(dst->depth());
  }
  tier0::create_dst(src, dst, width, height, depth);
  const KernelInfo    kernel = { "crop", kernel::crop };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "index0", start_x }, { "index1", start_y }, { "index2", start_z } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
paste_func(const Device::Pointer & device,
           const Array::Pointer &  src,
           Array::Pointer          dst,
           int                     destination_x,
           int                     destination_y,
           int                     destination_z) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "paste", kernel::paste };
  const ParameterList params = {
    { "src", src }, { "dst", dst }, { "scalar0", destination_x }, { "scalar1", destination_y }, { "scalar2", destination_z }
  };
  const RangeArray range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
pad_func(const Device::Pointer & device,
         const Array::Pointer &  src,
         Array::Pointer          dst,
         size_t                  size_x,
         size_t                  size_y,
         size_t                  size_z,
         float                   value,
         bool                    center) -> Array::Pointer
{

  tier0::create_dst(src, dst, size_x, size_y, size_z);
  dst->fill(value);

  const auto pad_x = std::abs(static_cast<int>(src->width()) - static_cast<int>(size_x));
  const auto pad_y = std::abs(static_cast<int>(src->height()) - static_cast<int>(size_y));
  const auto pad_z = std::abs(static_cast<int>(src->depth()) - static_cast<int>(size_z));

  cle::RangeArray offset = { 0, 0, 0 };
  if (center)
  {
    offset = { (dst->width() > 1) ? static_cast<size_t>(std::ceil(pad_x / 2.0)) : 0,
               (dst->height() > 1) ? static_cast<size_t>(std::ceil(pad_y / 2.0)) : 0,
               (dst->depth() > 1) ? static_cast<size_t>(std::ceil(pad_z / 2.0)) : 0 };
  }
  src->copyTo(dst, { src->width(), src->height(), src->depth() }, { 0, 0, 0 }, offset);
  return dst;
}

auto
unpad_func(const Device::Pointer & device,
           const Array::Pointer &  src,
           Array::Pointer          dst,
           size_t                  size_x,
           size_t                  size_y,
           size_t                  size_z,
           bool                    center) -> Array::Pointer
{
  tier0::create_dst(src, dst, size_x, size_y, size_z);

  const auto pad_x = std::abs(static_cast<int>(src->width()) - static_cast<int>(size_x));
  const auto pad_y = std::abs(static_cast<int>(src->height()) - static_cast<int>(size_y));
  const auto pad_z = std::abs(static_cast<int>(src->depth()) - static_cast<int>(size_z));

  cle::RangeArray offset = { 0, 0, 0 };
  if (center)
  {
    offset = { (dst->width() > 1) ? static_cast<size_t>(std::ceil(pad_x / 2.0)) : 0,
               (dst->height() > 1) ? static_cast<size_t>(std::ceil(pad_y / 2.0)) : 0,
               (dst->depth() > 1) ? static_cast<size_t>(std::ceil(pad_z / 2.0)) : 0 };
  }
  src->copyTo(dst, { dst->width(), dst->height(), dst->depth() }, offset, { 0, 0, 0 });
  return dst;
}

auto
flip_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, bool flip_x, bool flip_y, bool flip_z)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "flip", kernel::flip };
  const ParameterList params = {
    { "src", src }, { "dst", dst }, { "index0", int(flip_x) }, { "index1", int(flip_y) }, { "index2", int(flip_z) }
  };
  const RangeArray range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
circular_shift_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    const int               shift_x,
                    const int               shift_y,
                    const int               shift_z) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "circular_shift", kernel::circular_shift };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "index_x", shift_x }, { "index_y", shift_y }, { "index_z", shift_z } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return dst;
}

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
