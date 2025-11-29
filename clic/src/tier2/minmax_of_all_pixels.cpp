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

  auto project_if_needed = [&](auto projection_func, int dimension) {
    if (dimension > 1)
    {
      tmp = projection_func(device, tmp, nullptr);
    }
  };

  project_if_needed(tier1::maximum_z_projection_func, tmp->depth());
  project_if_needed(tier1::maximum_y_projection_func, tmp->height());
  tier1::maximum_x_projection_func(device, tmp, dst);

  float res;
  dst->readTo(&res);
  return res;
}

auto
minimum_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float
{
  Array::Pointer dst = nullptr;
  Array::Pointer tmp = src;
  tier0::create_one(src, dst, dType::FLOAT);

  auto project_if_needed = [&](auto projection_func, int dimension) {
    if (dimension > 1)
    {
      tmp = projection_func(device, tmp, nullptr);
    }
  };

  project_if_needed(tier1::minimum_z_projection_func, tmp->depth());
  project_if_needed(tier1::minimum_y_projection_func, tmp->height());
  tier1::minimum_x_projection_func(device, tmp, dst);

  float res;
  dst->readTo(&res);
  return res;
}

} // namespace cle::tier2
