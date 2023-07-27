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
// rotate
// scale
// translate

auto
voronoi_otsu_labeling_func(const Device::Pointer & device,
                           const Array::Pointer &  src,
                           Array::Pointer          dst,
                           float                   spot_sigma,
                           float                   outline_sigma) -> Array::Pointer;

} // namespace cle::tier7

#endif // __INCLUDE_TIER7_HPP
