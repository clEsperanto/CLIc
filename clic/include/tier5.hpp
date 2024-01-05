#ifndef __INCLUDE_TIER5_HPP
#define __INCLUDE_TIER5_HPP

#include "tier0.hpp"

/**
 * @namespace cle::tier5
 * @brief Namespace container for all functions of tier 2 category
 *        Tier 5 functions are advanced functions that may rely on previous tier functions
 */
namespace cle::tier5
{

/**
 * @name array_equal
 * @brief Compares if all pixels of two images are identical. If shape of the images or any pixel are different, returns
 * False. True otherwise  This function is supposed to work similarly like its counterpart in numpy [1].
 * @category 'combine'
 * @link https://numpy.org/doc/stable/reference/generated/numpy.array_equal.html
 */
auto
array_equal_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1) -> bool;


/**
 * @name combine_labels
 * @brief Combines two label images by adding labels of a given label image to another. Labels in the second image
 overwrite labels in the first passed image. Afterwards, labels are relabeled sequentially.
 * @category 'label processing', 'combine labels', 'in assistant', 'bia-bob-suggestion'
 */
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
