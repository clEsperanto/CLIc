
#include "tier7.hpp"
#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"
#include "tier6.hpp"

#include "transform.hpp"

namespace cle::tier7
{

// auto
// affine_transform_func(const Device::Pointer & device,
//                       const Array::Pointer &  src,
//                       Array::Pointer          dst,
//                       std::array<float, 16>   transform,
//                       bool                    interpolate,
//                       bool                    resize) -> Array::Pointer
// {
//   auto transform_matrix = AffineTransform(transform);
//   return apply_affine_transform(src, dst, transform_matrix, interpolate, resize);
// }


auto
rigid_transform_func(const Device::Pointer & device,
                     const Array::Pointer &  src,
                     Array::Pointer          dst,
                     float                   translate_x,
                     float                   translate_y,
                     float                   translate_z,
                     float                   angle_x,
                     float                   angle_y,
                     float                   angle_z,
                     bool                    centered,
                     bool                    interpolate,
                     bool                    resize) -> Array::Pointer
{
  auto transform = AffineTransform();
  if (centered)
  {
    transform.center({ src->width(), src->height(), src->depth() }, false);
  }
  if (angle_x != 0)
  {
    transform.rotate(0, angle_x);
  }
  if (angle_y != 0)
  {
    transform.rotate(1, angle_y);
  }
  if (angle_z != 0)
  {
    transform.rotate(2, angle_z);
  }
  if (centered)
  {
    transform.center({ src->width(), src->height(), src->depth() }, true);
  }
  transform.translate(translate_x, translate_y, translate_z);
  return apply_affine_transform(src, dst, transform, interpolate, resize);
}

auto
rotate_func(const Device::Pointer & device,
            const Array::Pointer &  src,
            Array::Pointer          dst,
            float                   angle_x,
            float                   angle_y,
            float                   angle_z,
            bool                    centered,
            bool                    interpolate,
            bool                    resize) -> Array::Pointer
{
  auto transform = AffineTransform();
  if (centered)
  {
    transform.center({ src->width(), src->height(), src->depth() }, false);
  }
  if (angle_x != 0)
  {
    transform.rotate(0, angle_x);
  }
  if (angle_y != 0)
  {
    transform.rotate(1, angle_y);
  }
  if (angle_z != 0)
  {
    transform.rotate(2, angle_z);
  }
  if (centered)
  {
    transform.center({ src->width(), src->height(), src->depth() }, true);
  }
  return apply_affine_transform(src, dst, transform, interpolate, resize);
}

auto
scale_func(const Device::Pointer & device,
           const Array::Pointer &  src,
           Array::Pointer          dst,
           float                   factor_x,
           float                   factor_y,
           float                   factor_z,
           bool                    centered,
           bool                    interpolate,
           bool                    resize) -> Array::Pointer
{
  auto transform = AffineTransform();
  if (centered && !resize)
  {
    transform.center({ src->width(), src->height(), src->depth() }, false);
  }
  transform.scale(factor_x, factor_y, factor_z);
  if (centered && !resize)
  {
    transform.center({ src->width(), src->height(), src->depth() }, true);
  }
  return apply_affine_transform(src, dst, transform, interpolate, resize);
}


auto
translate_func(const Device::Pointer & device,
               const Array::Pointer &  src,
               Array::Pointer          dst,
               float                   translate_x,
               float                   translate_y,
               float                   translate_z,
               bool                    interpolate) -> Array::Pointer
{
  auto transform = AffineTransform();
  transform.translate(translate_x, translate_y, translate_z);
  return apply_affine_transform(src, dst, transform, interpolate, false);
}

auto
closing_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::UINT32);
  if (radius == 0)
  {
    return tier1::copy_func(device, src, dst);
  }

  auto temp = tier6::dilate_labels_func(device, src, nullptr, radius);

  auto flip = tier1::greater_constant_func(device, temp, nullptr, 0);
  auto flop = Array::create(flip);

  for (size_t i = 0; i < radius; i++)
  {
    if (i % 2 == 0)
    {
      tier1::erode_sphere_func(device, flip, flop);
    }
    else
    {
      tier1::erode_box_func(device, flip, flop);
    }
  }
  if (radius % 2 == 0)
  {
    return tier1::multiply_images_func(device, flip, temp, dst);
  }
  return tier1::multiply_images_func(device, flop, temp, dst);
}

auto
opening_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::UINT32);
  auto temp = tier6::erode_labels_func(device, src, nullptr, radius, true);
  return tier6::dilate_labels_func(device, temp, dst, radius);
}

auto
voronoi_otsu_labeling_func(const Device::Pointer & device,
                           const Array::Pointer &  src,
                           Array::Pointer          dst,
                           float                   spot_sigma,
                           float                   outline_sigma) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::UINT32);
  auto temp = tier1::gaussian_blur_func(device, src, nullptr, spot_sigma, spot_sigma, spot_sigma);
  auto spot = tier2::detect_maxima_box_func(device, temp, nullptr, 0, 0, 0);
  temp = tier1::gaussian_blur_func(device, src, nullptr, outline_sigma, outline_sigma, outline_sigma);
  auto segmentation = tier4::threshold_otsu_func(device, temp, nullptr);
  auto binary = tier1::binary_and_func(device, spot, segmentation, nullptr);
  temp = tier6::masked_voronoi_labeling_func(device, binary, segmentation, nullptr);
  return tier1::mask_func(device, temp, segmentation, dst);
}


} // namespace cle::tier7
