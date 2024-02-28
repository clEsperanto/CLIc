#ifndef __INCLUDE_TIER8_HPP
#define __INCLUDE_TIER8_HPP

#include "tier0.hpp"

/**
 * @namespace cle::tier8
 * @brief Namespace container for all functions of tier 8 category
 *        Tier 8 functions are advanced functions that may rely on previous tier functions
 */
namespace cle::tier8
{

/**
 * @name smooth_labels
 * @category 'label processing', 'in assistant', 'bia-bob-suggestion'
 * @brief Apply a morphological opening operation to a label image and afterwards
 *   fills gaps between the labels using voronoi-labeling. Finally, the result
 *   label image is masked so that all background pixels remain background pixels.
 *
 *   Note: It is recommended to process isotropic label images.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @param radius [int ( = 0 )]
 * @return Array::Pointer
 *
 */
auto
smooth_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius)
  -> Array::Pointer;


} // namespace cle::tier8

#endif // __INCLUDE_TIER8_HPP
