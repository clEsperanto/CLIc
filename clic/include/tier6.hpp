#ifndef __INCLUDE_TIER6_HPP
#define __INCLUDE_TIER6_HPP

#include "array.hpp"
#include "device.hpp"
#include "execution.hpp"

namespace cle::tier6
{

auto
masked_voronoi_labeling_func(const Device::Pointer & device,
                             const Array::Pointer &  src,
                             const Array::Pointer &  mask,
                             Array::Pointer          dst) -> Array::Pointer;

auto
voronoi_labeling_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


} // namespace cle::tier6

#endif // __INCLUDE_TIER6_HPP
