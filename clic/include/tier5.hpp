#ifndef __INCLUDE_TIER5_HPP
#define __INCLUDE_TIER5_HPP

#include "array.hpp"
#include "device.hpp"
#include "execution.hpp"

namespace cle::tier5
{

// array_equal
// closing_labels
// connected_components_labeling_diamond
// fill_zeros_inpainting
// generate_touch_mean_intensity_within_range_matrix
// is_matrix_symmetric
// merge_labels_according_to_touch_matrix
// opening_labels


auto
combine_labels_func(const Device::Pointer & device,
                    const Array::Pointer &  src0,
                    const Array::Pointer &  src1,
                    Array::Pointer          dst) -> Array::Pointer;


auto
connected_components_labeling_box_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


} // namespace cle::tier5

#endif // __INCLUDE_TIER5_HPP
