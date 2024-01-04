#ifndef __INCLUDE_TIER7_HPP
#define __INCLUDE_TIER7_HPP

#include "array.hpp"
#include "device.hpp"
#include "execution.hpp"

/**
 * @namespace cle::tier7
 * @brief Contains all the functions of the tier 7 category.
 *
 */
namespace cle::tier7
{
// auto
// affine_transform_func(const Device::Pointer & device,
//                       const Array::Pointer &  src,
//                       Array::Pointer          dst,
//                       float                   translate_x,
//                       float                   translate_y,
//                       float                   translate_z,
//                       float                   angle_x,
//                       float                   angle_y,
//                       float                   angle_z,
//                       float                   factor_x,
//                       float                   factor_y,
//                       float                   factor_z,
//                       bool                    centered,
//                       bool                    interpolate,
//                       bool                    resize) -> Array::Pointer;

// apply_vector_field
// deskew_y
// deskew_x

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
                     bool                    resize) -> Array::Pointer;

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
