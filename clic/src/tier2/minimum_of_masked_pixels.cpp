#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp" 

#include "utils.hpp"

namespace cle::tier2 
{

auto
minimum_of_masked_pixels_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & mask)
  -> float
{
  Array::Pointer dst_src = nullptr;
  Array::Pointer dst_mask = nullptr;

  Array::Pointer tmp_src = src;
  Array::Pointer tmp_mask = mask;
  if (tmp_src->depth() > 1)
  {
    dst_src = nullptr;
    dst_mask = nullptr;
    tier0::create_xy(src, dst_src);
    tier0::create_xy(mask, dst_mask);
    tier1::minimum_of_masked_pixels_reduction_func(device, tmp_src, tmp_mask, dst_src, dst_mask);
    tmp_src = dst_src;
    tmp_mask = dst_mask;
  }
  if (tmp_src->height() > 1)
  {
    dst_src = nullptr;
    dst_mask = nullptr;
    tmp_src = tier1::transpose_yz_func(device, tmp_src, nullptr);
    tmp_mask = tier1::transpose_yz_func(device, tmp_mask, nullptr);
    tier0::create_vector(tmp_src, dst_src, tmp_src->width());
    tier0::create_vector(tmp_mask, dst_mask, tmp_mask->width());
    tier1::minimum_of_masked_pixels_reduction_func(device, tmp_src, tmp_mask, dst_src, dst_mask);
    tmp_src = dst_src;
    tmp_mask = dst_mask;
  }
  dst_src = nullptr;
  dst_mask = nullptr;
  tmp_src = tier1::transpose_xz_func(device, tmp_src, nullptr);
  tmp_mask = tier1::transpose_xz_func(device, tmp_mask, nullptr);
  tier0::create_one(tmp_src, dst_src, dType::FLOAT);
  tier0::create_one(tmp_mask, dst_mask, dType::FLOAT);
  tier1::minimum_of_masked_pixels_reduction_func(device, tmp_src, tmp_mask, dst_src, dst_mask);

  float res;
  dst_src->read(&res);
  return res;
}

// @StRigaud TODO: auto minimum_of_touching_neighbors_func;
// @StRigaud TODO: auto mode_of_touching_neighbors_func;
// @StRigaud TODO: auto neighbors_of_neighbors_func;  

}

