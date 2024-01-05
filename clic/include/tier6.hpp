#ifndef __INCLUDE_TIER6_HPP
#define __INCLUDE_TIER6_HPP

#include "tier0.hpp"

/**
 * @namespace cle::tier6
 * @brief Namespace container for all functions of tier 2 category
 *        Tier 6 functions are advanced functions that may rely on previous tier functions
 */
namespace cle::tier6
{

/**
 * @name dilate_labels
 * @brief Dilates labels to a larger size. No label overwrites another label. Similar to the implementation in
 * scikit-image [2] and MorpholibJ[3]  Notes ----- * This operation assumes input images are isotropic.
 * @category 'label processing', 'in assistant', 'bia-bob-suggestion'
 * @link https://clij.github.io/clij2-docs/reference_dilateLabels
 * @link https://scikit-image.org/docs/dev/auto_examples/segmentation/plot_expand_labels.html?highlight=expand%20labels
 * @link https://github.com/ijpb/MorphoLibJ
 */
auto
dilate_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius)
  -> Array::Pointer;


/**
 * @name erode_labels
 * @brief Erodes labels to a smaller size. Note: Depending on the label image and the radius, labels may disappear and
 * labels may split into multiple islands. Thus, overlapping labels of input and output may not have the same
 * identifier.  Notes ----- * This operation assumes input images are isotropic.
 * @category 'label processing', 'in assistant'
 * @link https://clij.github.io/clij2-docs/reference_erodeLabels
 */
auto
erode_labels_func(const Device::Pointer & device,
                  const Array::Pointer &  src,
                  Array::Pointer          dst,
                  int                     radius,
                  bool                    relabel) -> Array::Pointer;


/**
 * @name masked_voronoi_labeling
 * @brief Takes a binary image, labels connected components and dilates the regions using a octagon shape until they
 * touch. The region growing is limited to a masked area.  The resulting label map is written to the output.
 * @category 'label', 'bia-bob-suggestion'
 * @link https://clij.github.io/clij2-docs/reference_maskedVoronoiLabeling
 */
auto
masked_voronoi_labeling_func(const Device::Pointer & device,
                             const Array::Pointer &  src,
                             const Array::Pointer &  mask,
                             Array::Pointer          dst) -> Array::Pointer;


/**
 * @name voronoi_labeling
 * @brief Takes a binary image, labels connected components and dilates the regions using a octagon shape until they
 * touch.  The resulting label map is written to the output.
 * @category 'label', 'in assistant', 'bia-bob-suggestion'
 * @link https://clij.github.io/clij2-docs/reference_voronoiLabeling
 */
auto
voronoi_labeling_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


} // namespace cle::tier6

#endif // __INCLUDE_TIER6_HPP
