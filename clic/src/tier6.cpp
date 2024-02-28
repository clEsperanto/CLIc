
#include "tier6.hpp"
#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"

namespace cle::tier6
{

auto
dilate_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::UINT32);
  if (radius <= 0)
  {
    return tier1::copy_func(device, src, dst);
  }

  auto flip = tier1::copy_func(device, src, nullptr);
  auto flog = Array::create(flip);

  auto flag = Array::create(1, 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  flag->fill(0);

  int   iteration_count = 0;
  float flag_value = 1;
  while (flag_value > 0 && iteration_count < radius)
  {
    if (iteration_count % 2 == 0)
    {
      tier1::onlyzero_overwrite_maximum_box_func(device, flip, flag, flog);
    }
    else
    {
      tier1::onlyzero_overwrite_maximum_diamond_func(device, flog, flag, flip);
    }
    flag->read(&flag_value);
    flag->fill(0);
    iteration_count++;
  }

  if (iteration_count % 2 == 0)
  {
    tier1::copy_func(device, flip, dst);
  }
  else
  {
    tier1::copy_func(device, flog, dst);
  }
  return dst;
}

auto
erode_labels_func(const Device::Pointer & device,
                  const Array::Pointer &  src,
                  Array::Pointer          dst,
                  int                     radius,
                  bool                    relabel) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::UINT32);
  if (radius <= 0)
  {
    return tier1::copy_func(device, src, dst);
  }

  auto temp = tier1::detect_label_edges_func(device, src, nullptr);
  auto temp1 = tier1::binary_not_func(device, temp, nullptr);
  tier1::mask_func(device, src, temp1, temp);
  temp1.reset();

  if (radius == 1)
  {
    tier1::copy_func(device, temp, dst);
  }
  else
  {
    for (size_t i = 0; i < radius - 1; i++)
    {
      if (i % 2 == 0)
      {
        tier1::minimum_sphere_func(device, temp, dst, 1, 1, 1);
      }
      else
      {
        tier1::minimum_box_func(device, dst, temp, 1, 1, 1);
      }
    }
  }
  if (relabel)
  {
    if (radius % 2 != 0)
    {
      tier1::copy_func(device, temp, dst);
    }
    tier1::not_equal_constant_func(device, dst, temp, 0);
    tier5::connected_components_labeling_func(device, temp, dst, "diamond");
  }
  else
  {
    if (radius % 2 == 0)
    {
      tier1::copy_func(device, dst, temp);
    }
    tier4::relabel_sequential_func(device, temp, dst, 4096);
  }
  return dst;
}


auto gauss_otsu_labeling_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float outline_sigma ) 
  -> Array::Pointer {
  tier0::create_like(src, dst, dType::UINT32);

  auto temp = tier1::gaussian_blur_func(device, src, nullptr, outline_sigma, outline_sigma, outline_sigma);

  auto binary = tier4::threshold_otsu_func(device, temp, nullptr);

  tier5::connected_components_labeling_func(device, binary, dst, "box");

  return dst;
}


auto
masked_voronoi_labeling_func(const Device::Pointer & device,
                             const Array::Pointer &  src,
                             const Array::Pointer &  mask,
                             Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::UINT32);
  Array::Pointer flip = nullptr;
  Array::Pointer flop = nullptr;
  Array::Pointer flup = nullptr;
  tier0::create_like(src, flip, dType::FLOAT);
  tier0::create_like(src, flop, dType::FLOAT);
  tier0::create_like(src, flup, dType::FLOAT);

  tier1::add_image_and_scalar_func(device, mask, flup, -1);
  tier5::connected_components_labeling_func(device, src, flop, "box");
  tier1::add_images_weighted_func(device, flop, flup, flip, 1, 1);
  auto flag = Array::create(1, 1, 1, 1, dType::INT32, mType::BUFFER, device);
  flag->fill(1);
  int flag_value = 1;
  int iteration_count = 0;
  while (flag_value > 0)
  {
    if (iteration_count % 2 == 0)
    {
      tier1::onlyzero_overwrite_maximum_box_func(device, flip, flag, flop);
    }
    else
    {
      tier1::onlyzero_overwrite_maximum_diamond_func(device, flop, flag, flip);
    }
    flag->read(&flag_value);
    flag->fill(0);
    iteration_count++;
  }
  if (iteration_count % 2 == 0)
  {
    tier1::mask_func(device, flip, mask, dst);
  }
  else
  {
    tier1::mask_func(device, flop, mask, dst);
  }
  return dst;
}

auto
voronoi_labeling_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::UINT32);
  auto flip = tier5::connected_components_labeling_func(device, src, nullptr, "box");
  return tier2::extend_labeling_via_voronoi_func(device, flip, dst);
}

} // namespace cle::tier6
