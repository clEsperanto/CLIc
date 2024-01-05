#ifndef __INCLUDE_TIER4_HPP
#define __INCLUDE_TIER4_HPP

#include "tier0.hpp"

/**
 * @namespace cle::tier4
 * @brief Namespace container for all functions of tier 2 category
 *        Tier 4 functions are advanced functions that may rely on previous tier functions
 */
namespace cle::tier4
{

/**
 * @name mean_squared_error
 * @brief Determines the mean squared error (MSE) between two images.  The MSE will be stored in a new row of ImageJs
 * Results table in the column 'MSE'.
 * @category 'in assistant', 'combine', 'bia-bob-suggestion'
 * @link https://clij.github.io/clij2-docs/reference_meanSquaredError
 */
auto
mean_squared_error_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1)
  -> float;


/**
 * @name spots_to_pointlist
 * @brief Transforms a spots image as resulting from maximum/minimum detection in an image where every column contains d
 * pixels (with d = dimensionality of the original image) with the coordinates of the maxima/minima.
 * @link https://clij.github.io/clij2-docs/reference_spotsToPointList
 */
auto
spots_to_pointlist_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name relabel_sequential
 * @brief Analyses a label map and if there are gaps in the indexing (e.g. label 5 is not present) all subsequent labels
 * will be relabelled.  Thus, afterwards number of labels and maximum label index are equal. This operation is mostly
 * performed on the CPU.
 * @category 'label processing', 'in assistant', 'bia-bob-suggestion'
 * @link https://clij.github.io/clij2-docs/reference_closeIndexGapsInLabelMap
 */
auto
relabel_sequential_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int blocksize)
  -> Array::Pointer;


auto
threshold_otsu_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

} // namespace cle::tier4

#endif // __INCLUDE_TIER4_HPP
