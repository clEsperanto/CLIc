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
  return tier1::crop_func(device, src, dst, 0, 0, start_z, 0, 0, nb_slice);
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

} // namespace cle::tier2
