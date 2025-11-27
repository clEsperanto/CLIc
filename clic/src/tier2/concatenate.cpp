#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

namespace cle::tier2
{

auto
concatenate_along_x_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_dst(src0, dst, src0->width() + src1->width(), src0->height(), src0->depth(), src0->dtype());
  tier1::paste_func(device, src0, dst, 0, 0, 0);
  tier1::paste_func(device, src1, dst, src0->width(), 0, 0);
  return dst;
}

auto
concatenate_along_y_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_dst(src0, dst, src0->width(), src0->height() + src1->height(), src0->depth(), src0->dtype());
  tier1::paste_func(device, src0, dst, 0, 0, 0);
  tier1::paste_func(device, src1, dst, 0, src0->height(), 0);
  return dst;
}

auto
concatenate_along_z_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_dst(src0, dst, src0->width(), src0->height(), src0->depth() + src1->depth(), src0->dtype());
  tier1::paste_func(device, src0, dst, 0, 0, 0);
  tier1::paste_func(device, src1, dst, 0, 0, src0->depth());
  return dst;
}

auto
concatenate(const Device::Pointer & device,
            const Array::Pointer &  src0,
            const Array::Pointer &  src1,
            Array::Pointer          dst,
            const int               axis) -> Array::Pointer
{
  switch (axis)
  {
    case 0: return concatenate_along_x_func(device, src0, src1, dst);
    case 1: return concatenate_along_y_func(device, src0, src1, dst);
    case 2: return concatenate_along_z_func(device, src0, src1, dst);
    default:
      throw std::invalid_argument("concatenate: axis must be 0, 1 or 2");
  }
} // namespace cle::tier2
