#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp" 

#include "utils.hpp"

namespace cle::tier2 
{

auto
maximum_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float
{
  Array::Pointer dst = nullptr;
  Array::Pointer tmp = src;
  tier0::create_one(src, dst, dType::FLOAT);
  if (src->depth() > 1)
  {
    auto proj_z = tier1::maximum_z_projection_func(device, tmp, nullptr);
    tmp = proj_z;
  }
  if (src->height() > 1)
  {
    auto proj_y = tier1::maximum_y_projection_func(device, tmp, nullptr);
    tmp = proj_y;
  }
  tier1::maximum_x_projection_func(device, tmp, dst);
  float res;
  dst->read(&res);
  return res;
}

// @StRigaud TODO: auto maximum_of_touching_neighbors_func;
// @StRigaud TODO: auto mean_of_touching_neighbors_func;
// @StRigaud TODO: auto median_of_touching_neighbors_func;  

}

