#ifndef __INCLUDE_TIER7_HPP
#define __INCLUDE_TIER7_HPP

#include "array.hpp"
#include "device.hpp"
#include "execution.hpp"

namespace cle::tier7
{
// affine_transform
// AffineTransform3D
// apply_vector_field
// deskew_y
// deskew_x
// rigid_transform

auto
rotate_func(const Device::Pointer & device,
            const Array::Pointer &  src,
            Array::Pointer          dst,
            float                   angle_x,
            float                   angle_y,
            float                   angle_z,
            bool                    centered,
            bool                    interpolate,
            bool                    resize) -> Array::Pointer;

auto
scale_func(const Device::Pointer & device,
           const Array::Pointer &  src,
           Array::Pointer          dst,
           float                   factor_x,
           float                   factor_y,
           float                   factor_z,
           bool                    centered,
           bool                    interpolate,
           bool                    resize) -> Array::Pointer;

auto
translate_func(const Device::Pointer & device,
               const Array::Pointer &  src,
               Array::Pointer          dst,
               float                   translate_x,
               float                   translate_y,
               float                   translate_z,
               bool                    interpolate) -> Array::Pointer;

auto
closing_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius)
  -> Array::Pointer;

auto
opening_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius)
  -> Array::Pointer;

auto
voronoi_otsu_labeling_func(const Device::Pointer & device,
                           const Array::Pointer &  src,
                           Array::Pointer          dst,
                           float                   spot_sigma,
                           float                   outline_sigma) -> Array::Pointer;

} // namespace cle::tier7

#endif // __INCLUDE_TIER7_HPP
