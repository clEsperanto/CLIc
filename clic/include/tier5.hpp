#ifndef __INCLUDE_TIER5_HPP
#define __INCLUDE_TIER5_HPP

#include "array.hpp"
#include "device.hpp"
#include "execution.hpp"

namespace cle::tier5
{

auto
array_equal(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1) -> bool;

// fill_zeros_inpainting
// generate_touch_mean_intensity_within_range_matrix
// is_matrix_symmetric
// merge_labels_according_to_touch_matrix


auto
combine_labels_func(const Device::Pointer & device,
                    const Array::Pointer &  src0,
                    const Array::Pointer &  src1,
                    Array::Pointer          dst) -> Array::Pointer;


auto
connected_components_labeling_func(const Device::Pointer & device,
                                   const Array::Pointer &  src,
                                   Array::Pointer          dst,
                                   const std::string &     connectivity) -> Array::Pointer;


} // namespace cle::tier5

#endif // __INCLUDE_TIER5_HPP
