#include "tier2.hpp"
#include "tier0.hpp"
#include "tier1.hpp"

namespace cle::tier2
{

auto
difference_of_gaussian_func(const Device::Pointer & device,
                            const Array::Pointer &  src,
                            Array::Pointer          dst,
                            const float &           sigma1_x,
                            const float &           sigma1_y,
                            const float &           sigma1_z,
                            const float &           sigma2_x,
                            const float &           sigma2_y,
                            const float &           sigma2_z) -> Array::Pointer
{
  tier0::create_like(src, dst);
  auto gauss1 = Array::create(dst);
  auto gauss2 = Array::create(dst);
  tier1::gaussian_blur_func(device, src, gauss1, sigma1_x, sigma1_y, sigma1_z);
  tier1::gaussian_blur_func(device, src, gauss2, sigma2_x, sigma2_y, sigma2_z);
  tier1::add_images_weighted_func(device, gauss1, gauss2, dst, 1, -1);
  return dst;
}

auto
maximum_all_pixels(const Device::Pointer & device, const Array::Pointer & src) -> float
{
  Array::Pointer proj_z;
  Array::Pointer proj_y;
  Array::Pointer proj_x;
  if (src->depth() > 1)
  {
    proj_z = tier1::maximum_z_projection_func(device, src, nullptr);
  }
  else
  {
    proj_z = src;
  }
  if (src->height() > 1)
  {
    proj_y = tier1::maximum_y_projection_func(device, src, nullptr);
  }
  else
  {
    proj_y = src;
  }
  proj_x = tier1::maximum_x_projection_func(device, src, nullptr);

  float result;
  proj_z->read(&result);
  return result;
}

} // namespace cle::tier2