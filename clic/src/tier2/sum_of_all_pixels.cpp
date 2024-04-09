#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp" 

#include "utils.hpp"

namespace cle::tier2 
{

auto
sum_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float
{
  Array::Pointer dst = nullptr;
  Array::Pointer tmp = src;
  tier0::create_one(src, dst, dType::FLOAT);
  if (src->depth() > 1)
  {
    auto proj_z = tier1::sum_z_projection_func(device, tmp, nullptr);
    tmp = proj_z;
  }
  if (src->height() > 1)
  {
    auto proj_y = tier1::sum_y_projection_func(device, tmp, nullptr);
    tmp = proj_y;
  }
  tier1::sum_x_projection_func(device, tmp, dst);
  float res;
  dst->read(&res);
  return res;
}

// @StRigaud TODO: auto symmetric_maximum_matrix_func;
// @StRigaud TODO: auto symmetric_mean_matrix_func;
// @StRigaud TODO: auto symmetric_minimum_matrix_func;
// @StRigaud TODO: auto symmetric_sum_matrix_func;  

}

