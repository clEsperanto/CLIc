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

  auto project_if_needed = [&](auto projection_func, int dimension) {
    if (dimension > 1)
    {
      tmp = projection_func(device, tmp, nullptr, false);
    }
  };

  project_if_needed(tier1::sum_z_projection_func, tmp->depth());
  project_if_needed(tier1::sum_y_projection_func, tmp->height());
  tier1::sum_x_projection_func(device, tmp, dst);

  float res;
  dst->readTo(&res);
  return res;
}

auto
product_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float
{
  Array::Pointer dst = nullptr;
  Array::Pointer tmp = src;
  tier0::create_one(src, dst, dType::FLOAT);

  auto project_if_needed = [&](auto projection_func, int dimension) {
    if (dimension > 1)
    {
      tmp = projection_func(device, tmp, nullptr, false);
    }
  };

  project_if_needed(tier1::product_z_projection_func, tmp->depth());
  project_if_needed(tier1::product_y_projection_func, tmp->height());
  tier1::product_x_projection_func(device, tmp, dst);

  float res;
  dst->readTo(&res);
  return res;
}

auto
maximum_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float
{
  Array::Pointer dst = nullptr;
  Array::Pointer tmp = src;
  tier0::create_one(src, dst, dType::FLOAT);

  auto project_if_needed = [&](auto projection_func, int dimension) {
    if (dimension > 1)
    {
      tmp = projection_func(device, tmp, nullptr, false);
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
      tmp = projection_func(device, tmp, nullptr, false);
    }
  };

  project_if_needed(tier1::minimum_z_projection_func, tmp->depth());
  project_if_needed(tier1::minimum_y_projection_func, tmp->height());
  tier1::minimum_x_projection_func(device, tmp, dst);

  float res;
  dst->readTo(&res);
  return res;
}

auto
minimum_of_masked_pixels_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & mask) -> float
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
  dst_src->readTo(&res);
  return res;
}

} // namespace cle::tier2
