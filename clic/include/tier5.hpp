#ifndef __INCLUDE_TIER5_HPP
#define __INCLUDE_TIER5_HPP

#include "tier0.hpp"

/**
 * @namespace cle::tier5
 * @brief Namespace container for all Tier 5 functions.
 *        Tier 5 functions are advanced and may rely on lower-tier functions.
 */
namespace cle::tier5
{
/**
 * @name array_equal
 * @brief Compares if all pixels of two images are identical. If the shape of the images or any pixel are different,
 * returns false; true otherwise. This function works similarly to its counterpart in NumPy.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 First array to compare. [const Array::Pointer &]
 * @param src1 Second array to compare. [const Array::Pointer &]
 * @return bool
 *
 * @note 'combine'
 * @see https://numpy.org/doc/stable/reference/generated/numpy.array_equal.html
 */
auto
array_equal_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1) -> bool;


/**
 * @name combine_labels
 * @brief Combines two label images by adding labels from one label image to another. Labels in the second image
 * overwrite labels in the first image. Afterward, labels are relabeled sequentially.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 Label image to add labels to. [const Array::Pointer &]
 * @param src1 Label image to add labels from. [const Array::Pointer &]
 * @param dst Output label image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 * @note 'label processing', 'combine labels', 'in assistant', 'bia-bob-suggestion'
 *
 */
auto
combine_labels_func(const Device::Pointer & device,
                    const Array::Pointer &  src0,
                    const Array::Pointer &  src1,
                    Array::Pointer          dst) -> Array::Pointer;


/**
 * @name connected_components_labeling
 * @brief Performs connected components analysis by inspecting the neighborhood of every pixel in a binary image and
 * generates a label map.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Binary image to label. [const Array::Pointer &]
 * @param dst Output label image. [Array::Pointer ( = None )]
 * @param connectivity Defines pixel neighborhood relationship, "box" or "sphere". [const std::string & ( = 'box' )]
 * @return Array::Pointer
 *
 * @note 'label', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_connectedComponentsLabelingBox
 * @deprecated This method is deprecated. Consider using connected_component_labeling() instead.
 */
auto
connected_components_labeling_func(const Device::Pointer & device,
                                   const Array::Pointer &  src,
                                   Array::Pointer          dst,
                                   const std::string &     connectivity) -> Array::Pointer;


/**
 * @name connected_component_labeling
 * @brief Performs connected components analysis by inspecting the neighborhood of every pixel in a binary image and
 * generates a label map.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Binary image to label. [const Array::Pointer &]
 * @param dst Output label image. [Array::Pointer ( = None )]
 * @param connectivity Defines pixel neighborhood relationship, "box" or "sphere". [const std::string & ( = 'box' )]
 * @return Array::Pointer
 *
 * @note 'label', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_connectedComponentsLabelingBox
 */
auto
connected_component_labeling_func(const Device::Pointer & device,
                                  const Array::Pointer &  src,
                                  Array::Pointer          dst,
                                  const std::string &     connectivity) -> Array::Pointer;


/**
 * @name reduce_labels_to_centroids
 * @brief Takes a label map and reduces each label to its centroid.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Label image to reduce. [const Array::Pointer &]
 * @param dst Output label image with centroids. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_reduceLabelsToCentroids
 */
auto
reduce_labels_to_centroids_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

/**
 * @name filter_label_by_size
 * @brief Filters labelled objects outside the min/max size range.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input label image. [const Array::Pointer &]
 * @param dst Output label image. [Array::Pointer ( = None )]
 * @param minimum_size Minimum size of labels to keep. [float ( = 0 )]
 * @param maximum_size Maximum size of labels to keep. [float ( = 100 )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_excludeLabelsOutsideSizeRange
 */
auto
filter_label_by_size_func(const Device::Pointer & device,
                          const Array::Pointer &  src,
                          Array::Pointer          dst,
                          float                   minimum_size,
                          float                   maximum_size) -> Array::Pointer;

/**
 * @name exclude_labels_outside_size_range
 * @brief Filters labelled objects outside the min/max size range.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input label image. [const Array::Pointer &]
 * @param dst Output label image. [Array::Pointer ( = None )]
 * @param minimum_size Minimum size of labels to keep. [float ( = 0 )]
 * @param maximum_size Maximum size of labels to keep. [float ( = 100 )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_excludeLabelsOutsideSizeRange
 */
auto
exclude_labels_outside_size_range_func(const Device::Pointer & device,
                                       const Array::Pointer &  src,
                                       Array::Pointer          dst,
                                       float                   minimum_size,
                                       float                   maximum_size) -> Array::Pointer;

/**
 * @name merge_touching_labels
 * @brief Merges touching labels of a label image and relabels the result sequentially.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input label image. [const Array::Pointer &]
 * @param dst Output label image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_excludeLabelsOutsideSizeRange
 */
auto
merge_touching_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/** 
 * @name proximal_neighbor_count
 * @brief From a label map, determines which labels are whithin a given distance range of each other
 * and returns the number of those in vector.
 * 
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input label image. [const Array::Pointer &]
 * @param dst Output parametric image. [Array::Pointer ( = None )]
 * @param min_distance Minimum distance to consider a neighbor. [float ( = -1 )]
 * @param max_distance Maximum distance to consider a neighbor. [float ( = -1 )]
 * @return Array::Pointer
 * 
 * @note 'label measurement'
 */
auto 
proximal_neighbor_count_func(const Device::Pointer & device,
                             const Array::Pointer &  src,
                             Array::Pointer          dst,
                             float                   min_distance,
                             float                   max_distance) -> Array::Pointer;




} // namespace cle::tier5

#endif // __INCLUDE_TIER5_HPP
