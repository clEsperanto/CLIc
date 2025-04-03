#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include <cmath>

namespace cle::tier1
{

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


} // namespace cle::tier1
