#ifndef __INCLUDE_TIER4_HPP
#define __INCLUDE_TIER4_HPP

#include "tier0.hpp"

/**
 * @namespace cle::tier4
 * @brief Namespace container for all functions of tier 4 category
 *        Tier 4 functions are advanced functions that may rely on previous tier functions
 */
namespace cle::tier4
{
/**
 * @name mean_squared_error
 * @category 'in assistant', 'combine', 'bia-bob-suggestion'
 * @brief Determines the mean squared error (MSE) between two images. The MSE will be stored in a new row of ImageJs
 * Results table in the column 'MSE'.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 [const Array::Pointer &]
 * @param src1 [const Array::Pointer &]
 * @return float
 *
 * @link https://clij.github.io/clij2-docs/reference_meanSquaredError
 */
auto
mean_squared_error_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1)
  -> float;


/**
 * @name spots_to_pointlist
 * @brief Transforms a spots image as resulting from maximum/minimum detection in an image where every column contains d
 * pixels (with d = dimensionality of the original image) with the coordinates of the maxima/minima.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @link https://clij.github.io/clij2-docs/reference_spotsToPointList
 */
auto
spots_to_pointlist_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name relabel_sequential
 * @category 'label processing', 'in assistant', 'bia-bob-suggestion'
 * @brief Analyses a label map and if there are gaps in the indexing (e.g. label 5 is not present) all subsequent labels
 * will be relabelled. Thus, afterwards number of labels and maximum label index are equal. This operation is mostly
 * performed on the CPU.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @param blocksize Renumbering is done in blocks for performance reasons. [int ( = 4096 )]
 * @return Array::Pointer
 *
 * @link https://clij.github.io/clij2-docs/reference_closeIndexGapsInLabelMap
 */
auto
relabel_sequential_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int blocksize)
  -> Array::Pointer;


/**
 * @name threshold_otsu
 * @priority 1
 * @category 'binarize', 'in assistant', 'bia-bob-suggestion'
 * @brief Binarizes an image using Otsu's threshold method [3] implemented in scikit-image[2] using a histogram
 * determined on the GPU to create binary images.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @link https://clij.github.io/clij2-docs/reference_thresholdOtsu
 * @link https://scikit-image.org/docs/dev/api/skimage.filters.html#skimage.filters.threshold_otsu
 * @link https://ieeexplore.ieee.org/document/4310076
 */
auto
threshold_otsu_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

} // namespace cle::tier4

#endif // __INCLUDE_TIER4_HPP
