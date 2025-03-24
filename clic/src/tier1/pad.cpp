#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

namespace cle::tier1
{

  auto
  pad(const Device::Pointer & device,
      const Array::Pointer &  src,
      Array::Pointer          dst,
      size_t                     pad_x,
      size_t                     pad_y,
      size_t                     pad_z,
      float                   value,
      bool center) -> Array::Pointer
{
  const size_t new_width = src->width() + pad_x;
  const size_t new_height = src->height() + pad_y;
  const size_t new_depth = src->depth() + pad_z;

  cle::RangeArray offset = { 0, 0, 0 };
  if (center)
  {
    offset = { pad_x / 2, pad_y / 2, pad_z / 2 };
  }

  tier0::create_dst(src, dst, new_width, new_height, new_depth);
  dst->fill(value);
  src->copyTo(dst, { src->width(), src->height(), src->depth() }, {0, 0, 0}, offset);
  return dst;
}

auto
unpad(const Device::Pointer & device,
    const Array::Pointer &  src,
    Array::Pointer          dst,
    size_t                     pad_x,
    size_t                     pad_y,
    size_t                     pad_z,
    bool center) -> Array::Pointer
{
const size_t new_width = src->width() - pad_x;
const size_t new_height = src->height() - pad_y;
const size_t new_depth = src->depth() - pad_z;

cle::RangeArray offset = { 0, 0, 0 };
if (center)
{
  offset = { pad_x / 2, pad_y / 2, pad_z / 2 };
}

tier0::create_dst(src, dst, new_width, new_height, new_depth);
src->copyTo(dst, { dst->width(), dst->height(), dst->depth() }, offset, {0, 0, 0});
return dst;
}



} // namespace cle::tier1
