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
 * @brief Apply a morphological opening operation to a label image and afterwards
 *   fills gaps between the labels using voronoi-labeling. Finally, the result
 *   label image is masked so that all background pixels remain background pixels.
 *
 *   Note: It is recommended to process isotropic label images.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input label image [const Array::Pointer &]
 * @param dst Output label image [Array::Pointer ( = None )]
 * @param radius Smoothing radius.[int ( = 0 )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant', 'bia-bob-suggestion'
 */
auto
smooth_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius)
  -> Array::Pointer;


/**
 * @name smooth_connected_labels
 * @category 'label processing', 'in assistant', 'bia-bob-suggestion'
 * @brief Apply a morphological erosion and dilation of the label image with respect to
 *     the connectivity of the labels.
 *
 *   Note: It is recommended to process isotropic label images.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input label image [const Array::Pointer &]
 * @param dst Output label image [Array::Pointer ( = None )]
 * @param radius Smoothing radius. [int ( = 0 )]
 * @return Array::Pointer
 *
 */
auto
smooth_connected_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius)
  -> Array::Pointer;


} // namespace cle::tier8

#endif // __INCLUDE_TIER8_HPP
