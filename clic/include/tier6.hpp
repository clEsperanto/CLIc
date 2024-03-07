#ifndef __INCLUDE_TIER6_HPP
#define __INCLUDE_TIER6_HPP

#include "tier0.hpp"

/**
 * @namespace cle::tier6
 * @brief Namespace container for all functions of tier 6 category
 *        Tier 6 functions are advanced functions that may rely on previous tier functions
 */
namespace cle::tier6
{
/**
 * @name dilate_labels
 * @brief Dilates labels to a larger size. No label overwrites another label. Similar to the implementation in
 * scikitimage [2] and MorpholibJ[3] Notes * This operation assumes input images are isotropic.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src label image to erode [const Array::Pointer &]
 * @param dst result [Array::Pointer ( = None )]
 * @param radius [int ( = 2 )]
 * @return Array::Pointer
 *
 * @note category 'label processing', 'in assistant', 'bia-bob-suggestion'
 */
auto
dilate_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius)
  -> Array::Pointer;


/**
 * @name erode_labels
 * @brief Erodes labels to a smaller size. Note: Depending on the label image and the radius, labels may disappear and
 * labels may split into multiple islands. Thus, overlapping labels of input and output may not have the same
 * identifier. Notes * This operation assumes input images are isotropic.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src result [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @param radius [int ( = 1 )]
 * @param relabel and all label indices exist. [bool ( = False )]
 * @return Array::Pointer
 *
 * @note category 'label processing', 'in assistant'
 */
auto
erode_labels_func(const Device::Pointer & device,
                  const Array::Pointer &  src,
                  Array::Pointer          dst,
                  int                     radius,
                  bool                    relabel) -> Array::Pointer;


/**
 * @name gauss_otsu_labeling
 * @brief Labels objects directly from grey-value images.
 *
 *  The outline_sigma parameter allows tuning the segmentation result. Under the hood,
 *  this filter applies a Gaussian blur, Otsu-thresholding [1] and connected component labeling [2]. The
 *  thresholded binary image is flooded using the Voronoi tesselation approach starting from the found local maxima.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 intensity image to add labels [const Array::Pointer &]
 * @param dst Output label image. [Array::Pointer ( = None )]
 * @param outline_sigma Gaussian blur sigma along all axes [float ( = 0 )]
 * @return Array::Pointer

 * @note category 'label', 'in assistant', 'bia-bob-suggestion'
 * @see https://ieeexplore.ieee.org/document/4310076
 * @see https://en.wikipedia.org/wiki/Connected-component_labeling
 *
 */
auto
gauss_otsu_labeling_func(const Device::Pointer & device,
                         const Array::Pointer &  src,
                         Array::Pointer          dst,
                         float                   outline_sigma) -> Array::Pointer;


/**
 * @name masked_voronoi_labeling
 * @brief Takes a binary image, labels connected components and dilates the regions using a octagon shape until they
 * touch. The region growing is limited to a masked area. The resulting label map is written to the output.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param mask [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note category 'label', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_maskedVoronoiLabeling
 */
auto
masked_voronoi_labeling_func(const Device::Pointer & device,
                             const Array::Pointer &  src,
                             const Array::Pointer &  mask,
                             Array::Pointer          dst) -> Array::Pointer;


/**
 * @name voronoi_labeling
 * @brief Takes a binary image, labels connected components and dilates the regions using a octagon shape until they
 * touch. The resulting label map is written to the output.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note category 'label', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_voronoiLabeling
 */
auto
voronoi_labeling_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


} // namespace cle::tier6

#endif // __INCLUDE_TIER6_HPP
