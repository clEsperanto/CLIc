#ifndef __INCLUDE_TIER5_HPP
#define __INCLUDE_TIER5_HPP

#include "tier0.hpp"

/**
 * @namespace cle::tier5
 * @brief Namespace container for all functions of tier 5 category
 *        Tier 5 functions are advanced functions that may rely on previous tier functions
 */
namespace cle::tier5
{
/**
 * @name array_equal
 * @brief Compares if all pixels of two images are identical. If shape of the images or any pixel are different, returns
 * False. True otherwise This function is supposed to work similarly like its counterpart in numpy [1].
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 First array to compare [const Array::Pointer &]
 * @param src1 Second array to compare [const Array::Pointer &]
 * @return bool
 *
 * @note 'combine'
 * @see https://numpy.org/doc/stable/reference/generated/numpy.array_equal.html
 */
auto
array_equal_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1) -> bool;


/**
 * @name combine_labels
 * @brief Combines two label images by adding labels of a given label image to another. Labels in the second image
 * overwrite labels in the first passed image. Afterwards, labels are relabeled sequentially.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 label image to add labels to. [const Array::Pointer &]
 * @param src1 label image to add labels from. [const Array::Pointer &]
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
 * @brief Performs connected components analysis inspecting the box neighborhood of every pixel to a binary image and
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
 * @brief Performs connected components analysis inspecting the box neighborhood of every pixel to a binary image and
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
 * @brief Take a label map and reduce each label to its centroid.
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
reduce_labels_to_centroids_func(const Device::Pointer & device,
                                const Array::Pointer &  src,
                                Array::Pointer          dst) -> Array::Pointer;

/**
 * @name filter_label_by_size
 * @brief Filter labelled objects outside of the min/max size range value.
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
 * @brief Filter labelled objects outside of the min/max size range value.
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
 * @brief Merge touching labels of a label image and relabel the result sequentially.
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
merge_touching_labels_func(const Device::Pointer & device,
                           const Array::Pointer &  src,
                           Array::Pointer          dst) -> Array::Pointer;


} // namespace cle::tier5

#endif // __INCLUDE_TIER5_HPP
