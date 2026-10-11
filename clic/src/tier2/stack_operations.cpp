#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "utils.hpp"

namespace cle::tier2
{

auto
sub_stack_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int start_z, int end_z) -> Array::Pointer
{
  auto nb_slice = end_z - start_z + 1;
  nb_slice = nb_slice < 1 ? 1 : nb_slice;
  return tier1::crop_func(device, src, dst, 0, 0, start_z, static_cast<int>(src->width()), static_cast<int>(src->height()), nb_slice);
}

auto
reduce_stack_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int reduction_factor, int offset)
  -> Array::Pointer
{
  reduction_factor = reduction_factor < 1 ? 1 : reduction_factor;
  auto num_slice = static_cast<int>(src->depth() / reduction_factor);

  tier0::create_dst(src, dst, src->width(), src->height(), num_slice, src->dtype());
  auto temp_slice = Array::create(src->width(), src->height(), 1, 2, src->dtype(), src->mtype(), device);
  for (auto z = 0; z < num_slice; z++)
  {
    tier1::copy_slice_func(device, src, temp_slice, z * reduction_factor + offset);
    tier1::copy_slice_func(device, temp_slice, dst, z);
  }
  return dst;
}

auto
concatenate_func(const Device::Pointer & device,
                 const Array::Pointer &  src0,
                 const Array::Pointer &  src1,
                 Array::Pointer          dst,
                 const int               axis) -> Array::Pointer
{
  auto promoted_type = promoteType(src0->dtype(), src1->dtype());
  switch (axis)
  {
    case 0: {
      tier0::create_dst(src0, dst, src0->width() + src1->width(), src0->height(), src0->depth(), promoted_type);
      dst->fill(0);
      tier1::paste_func(device, src0, dst, 0, 0, 0);
      tier1::paste_func(device, src1, dst, src0->width(), 0, 0);
      break;
    }
    case 1: {
      tier0::create_dst(src0, dst, src0->width(), src0->height() + src1->height(), src0->depth(), promoted_type);
      dst->fill(0);
      tier1::paste_func(device, src0, dst, 0, 0, 0);
      tier1::paste_func(device, src1, dst, 0, src0->height(), 0);
      break;
    }
    case 2: {
      tier0::create_dst(src0, dst, src0->width(), src0->height(), src0->depth() + src1->depth(), promoted_type);
      dst->fill(0);
      tier1::paste_func(device, src0, dst, 0, 0, 0);
      tier1::paste_func(device, src1, dst, 0, 0, src0->depth());
      break;
    }
    default:
      throw std::invalid_argument("concatenate: axis must be 0, 1 or 2");
  }
  return dst;
}

auto
concatenate_along_x_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  return concatenate_func(device, src0, src1, dst, 0);
}

auto
concatenate_along_y_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  return concatenate_func(device, src0, src1, dst, 1);
}

auto
concatenate_along_z_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  return concatenate_func(device, src0, src1, dst, 2);
}

auto
crop_border_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int border_size) -> Array::Pointer
{
  std::array<int, 3> region = { static_cast<int>(src->width()) - 2 * border_size,
                                static_cast<int>(src->height()) - 2 * border_size,
                                static_cast<int>(src->depth()) - 2 * border_size };
  std::transform(region.begin(), region.end(), region.begin(), [](int i) { return std::max(i, 0); });
  return tier1::crop_func(device, src, dst, border_size, border_size, border_size, region[0], region[1], region[2]);
}

// @StRigaud TODO: auto distance_matrix_to_mesh_func;

} // namespace cle::tier2
