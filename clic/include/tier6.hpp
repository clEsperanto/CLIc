#ifndef __INCLUDE_TIER6_HPP
#define __INCLUDE_TIER6_HPP

#include "tier0.hpp"

/**
 * @namespace cle::tier6
 * @brief Namespace container for all Tier 6 functions.
 *        Tier 6 functions are advanced and may rely on lower-tier functions.
 */
namespace cle::tier6
{
/**
 * @name dilate_labels
 * @brief Dilates labels to a larger size. No label overwrites another label. Similar to the implementation in
 * scikit-image and MorphoLibJ. Note: This operation assumes input images are isotropic.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input label image to dilate. [const Array::Pointer &]
 * @param dst Output label image. [Array::Pointer ( = None )]
 * @param radius Dilation radius. [int ( = 2 )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant', 'bia-bob-suggestion'
 */
auto
dilate_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius) -> Array::Pointer;


/**
 * @name erode_labels
 * @brief Erodes labels to a smaller size. Note: Depending on the label image and the radius, labels may disappear and
 * labels may split into multiple islands. Thus, overlapping labels of input and output may not have the same
 * identifier. This operation assumes input images are isotropic.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input label image. [const Array::Pointer &]
 * @param dst Output label image. [Array::Pointer ( = None )]
 * @param radius Erosion radius. [int ( = 1 )]
 * @param relabel Relabel the image, e.g., if objects disappear or split. [bool ( = False )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant'
 */
auto
erode_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius, bool relabel)
  -> Array::Pointer;


/**
 * @name gauss_otsu_labeling
 * @brief Labels objects directly from gray-value images.
 *
 *  The outline_sigma parameter allows tuning the segmentation result. Under the hood,
 *  this filter applies a Gaussian blur, Otsu thresholding, and connected component labeling. The
 *  thresholded binary image is flooded using the Voronoi tessellation approach starting from the found local maxima.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 Intensity image to segment. [const Array::Pointer &]
 * @param dst Output label image. [Array::Pointer ( = None )]
 * @param outline_sigma Gaussian blur sigma along all axes. [float ( = 0 )]
 * @return Array::Pointer
 *
 * @note 'label', 'in assistant'
 * @see https://ieeexplore.ieee.org/document/4310076
 * @see https://en.wikipedia.org/wiki/Connected-component_labeling
 */
auto
gauss_otsu_labeling_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float outline_sigma)
  -> Array::Pointer;


/**
 * @name masked_voronoi_labeling
 * @brief Takes a binary image, labels connected components, and dilates the regions using an octagon shape until they
 * touch. The region growing is limited to a masked area. The resulting label map is written to the output.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input binary image. [const Array::Pointer &]
 * @param mask Input mask. [const Array::Pointer &]
 * @param dst Output label image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'label'
 * @see https://clij.github.io/clij2-docs/reference_maskedVoronoiLabeling
 */
auto
masked_voronoi_labeling_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & mask, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name voronoi_labeling
 * @brief Takes a binary image, labels connected components, and dilates the regions using an octagon shape until they
 * touch. The resulting label map is written to the output.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param input_binary Input binary image. [const Array::Pointer &]
 * @param output_labels Output label image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'label', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_voronoiLabeling
 */
auto
voronoi_labeling_func(const Device::Pointer & device, const Array::Pointer & input_binary, Array::Pointer output_labels) -> Array::Pointer;


/**
 * @name remove_small_labels
 * @brief Removes labelled objects smaller than a given size (in pixels) from a label map.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Label image to filter. [const Array::Pointer &]
 * @param dst Filtered output label image. [Array::Pointer ( = None )]
 * @param minimum_size Smallest size object allowed. [float ( = 100 )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_excludeLabelsOutsideSizeRange
 */
auto
remove_small_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float minimum_size)
  -> Array::Pointer;

/**
 * @name exclude_small_labels
 * @brief Removes labels from a label map that are below a given maximum size.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Label image to filter. [const Array::Pointer &]
 * @param dst Filtered output label image. [Array::Pointer ( = None )]
 * @param maximum_size Largest size object to exclude. [float ( = 100 )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_excludeLabelsOutsideSizeRange
 */
auto
exclude_small_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float maximum_size)
  -> Array::Pointer;

/**
 * @name remove_large_labels
 * @brief Removes labelled objects larger than a given size (in pixels) from a label map.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Label image to filter. [const Array::Pointer &]
 * @param dst Filtered output label image. [Array::Pointer ( = None )]
 * @param maximum_size Largest size object allowed. [float ( = 100 )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_excludeLabelsOutsideSizeRange
 */
auto
remove_large_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float maximum_size)
  -> Array::Pointer;

/**
 * @name exclude_large_labels
 * @brief Removes labels from a label map that are above a given minimum size.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Label image to filter. [const Array::Pointer &]
 * @param dst Filtered output label image. [Array::Pointer ( = None )]
 * @param minimum_size Smallest size object to exclude. [float ( = 100 )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_excludeLabelsOutsideSizeRange
 */
auto
exclude_large_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float minimum_size)
  -> Array::Pointer;


/**
 * @name proximal_neighbor_count_map
 * @brief From a label map, generates a map where each label is replaced by the count of neighboring labels within a specified distance
 * range.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param labels Input label image. [const Array::Pointer &]
 * @param dst Output parametric image. [Array::Pointer ( = None )]
 * @param min_distance Minimum distance to consider a neighbor. [float ( = -1 )]
 * @param max_distance Maximum distance to consider a neighbor. [float ( = -1 )]
 * @return Array::Pointer
 *
 * @note 'label measurement', 'map', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_proximalNeighborCountMap
 */
auto
proximal_neighbor_count_map_func(const Device::Pointer & device,
                                 const Array::Pointer &  labels,
                                 Array::Pointer          dst,
                                 float                   min_distance,
                                 float                   max_distance) -> Array::Pointer;



} // namespace cle::tier6

#endif // __INCLUDE_TIER6_HPP
