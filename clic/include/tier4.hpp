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
 * @name label_bounding_box
 * @brief Determines the bounding box of the specified label from a label image. The positions are returned in
 *  an array of 6 values as follows: minX, minY, minZ, maxX, maxY, maxZ.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param label_id [int]
 * @return std::array<float, 6>
 *
 * @see https://clij.github.io/clij2-docs/reference_boundingBox
 *
 */
auto
label_bounding_box_func(const Device::Pointer & device, const Array::Pointer & src, int label_id) -> std::vector<float>;

/**
 * @name mean_squared_error
 * @brief Determines the mean squared error (MSE) between two images. The MSE will be stored in a new row of ImageJs
 * Results table in the column 'MSE'.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 [const Array::Pointer &]
 * @param src1 [const Array::Pointer &]
 * @return float
 *
 * @note 'in assistant', 'combine', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_meanSquaredError
 */
auto
mean_squared_error_func(const Device::Pointer & device,
                        const Array::Pointer &  src0,
                        const Array::Pointer &  src1) -> float;


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
 * @see https://clij.github.io/clij2-docs/reference_spotsToPointList
 *
 */
auto
spots_to_pointlist_func(const Device::Pointer & device,
                        const Array::Pointer &  src,
                        Array::Pointer          dst) -> Array::Pointer;


/**
 * @name relabel_sequential
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
 * @note 'label processing', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_closeIndexGapsInLabelMap
 */
auto
relabel_sequential_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int blocksize)
  -> Array::Pointer;


/**
 * @name threshold_otsu
 * @brief Binarizes an image using Otsu's threshold method [3] implemented in scikit-image[2] using a histogram
 * determined on the GPU to create binary images.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'binarize', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_thresholdOtsu
 * @see https://scikit-image.org/docs/dev/api/skimage.filters.html#skimage.filters.threshold_otsu
 * @see https://ieeexplore.ieee.org/document/4310076
 */
auto
threshold_otsu_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

} // namespace cle::tier4

#endif // __INCLUDE_TIER4_HPP
