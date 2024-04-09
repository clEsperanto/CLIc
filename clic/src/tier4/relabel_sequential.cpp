#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp" 

#include "utils.hpp"

namespace cle::tier4 
{

auto
relabel_sequential_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int blocksize)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  auto max_label = static_cast<int>(tier2::maximum_of_all_pixels_func(device, src));
  auto flagged = Array::create(int(max_label + 1), 1, 1, 1, src->dtype(), src->mtype(), src->device());
  flagged->fill(0);
  tier3::flag_existing_labels_func(device, src, flagged);
  tier1::set_column_func(device, flagged, 0, 0);
  auto block_sums =
    Array::create(((max_label + 1) / blocksize) + 1, 1, 1, 1, flagged->dtype(), flagged->mtype(), flagged->device());
  tier1::sum_reduction_x_func(device, flagged, block_sums, blocksize);
  auto new_indices = Array::create(max_label + 1, 1, 1, 1, flagged->dtype(), flagged->mtype(), flagged->device());
  tier1::block_enumerate_func(device, flagged, block_sums, new_indices, blocksize);
  tier1::replace_values_func(device, src, new_indices, dst);
  return dst;
}  

}

