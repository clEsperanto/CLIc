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
 * @param src0 [const Array::Pointer &]
 * @param src1 [const Array::Pointer &]
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
 * @note 'label', 'in assistant', 'bia-bob-suggestion'
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
 * @name remove_small_labels
 * @brief Removes labelled objects small than a given size (in pixels) from a label map.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Label image to filter. [const Array::Pointer &]
 * @param dst Output label image fitlered. [Array::Pointer ( = None )]
 * @param min_size Smallest size object allowed. [float ( = 100 )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_excludeLabelsOutsideSizeRange
 */
auto
remove_small_labels_func(const Device::Pointer & device,
                          const Array::Pointer &  src,
                          Array::Pointer          dst,
                          float                   min_size) -> Array::Pointer;

/**
 * @name exclude_small_labels
 * @brief Removes labels from a label map which are below a given maximum size.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Label image to filter. [const Array::Pointer &]
 * @param dst Output label image fitlered. [Array::Pointer ( = None )]
 * @param max_size Largest size object to exclude. [float ( = 100 )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_excludeLabelsOutsideSizeRange
 */
auto
exclude_small_labels_func(const Device::Pointer & device,
                          const Array::Pointer &  src,
                          Array::Pointer          dst,
                          float                   max_size) -> Array::Pointer;                          

/**
 * @name remove_large_labels
 * @brief Removes labelled objects bigger than a given size (in pixels) from a label map.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Label image to filter. [const Array::Pointer &]
 * @param dst Output label image fitlered. [Array::Pointer ( = None )]
 * @param max_size Biggest size object allowed. [float ( = 100 )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_excludeLabelsOutsideSizeRange
 */
auto
remove_large_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float max_size)
  -> Array::Pointer;

/**
 * @name exclude_large_labels
 * @brief Removes labels from a label map which are higher a given minimum size.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Label image to filter. [const Array::Pointer &]
 * @param dst Output label image fitlered. [Array::Pointer ( = None )]
 * @param min_size Smallest size object to keep. [float ( = 100 )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_excludeLabelsOutsideSizeRange
 */
auto
exclude_large_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float min_size)
  -> Array::Pointer;  


} // namespace cle::tier5

#endif // __INCLUDE_TIER5_HPP
