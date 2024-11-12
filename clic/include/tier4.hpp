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
 * @param src Label image [const Array::Pointer &]
 * @param label_id Identifier of label [int]
 * @return std::vector<float>
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
 * @param src0 First image to compare [const Array::Pointer &]
 * @param src1 Second image to compare [const Array::Pointer &]
 * @return float
 *
 * @note 'in assistant', 'combine', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_meanSquaredError
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
 * @param src Input binary image of spots [const Array::Pointer &]
 * @param dst Output coordinate list of spots [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @see https://clij.github.io/clij2-docs/reference_spotsToPointList
 *
 */
auto
spots_to_pointlist_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name relabel_sequential
 * @brief Analyses a label map and if there are gaps in the indexing (e.g. label 5 is not present) all subsequent labels
 * will be relabelled. Thus, afterwards number of labels and maximum label index are equal. This operation is mostly
 * performed on the CPU.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input label image. [const Array::Pointer &]
 * @param dst Output label image. [Array::Pointer ( = None )]
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
 * @param src Input image to threshold. [const Array::Pointer &]
 * @param dst Output binary image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'binarize', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_thresholdOtsu
 * @see https://scikit-image.org/docs/dev/api/skimage.filters.html#skimage.filters.threshold_otsu
 * @see https://ieeexplore.ieee.org/document/4310076
 */
auto
threshold_otsu_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name mean_intensity_map
 * @brief Takes an image and a corresponding label map, determines the mean
 *   intensity per label and replaces every label with the that number.
 *
 * This results in a parametric image expressing mean object intensity.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src intensity image [const Array::Pointer &]
 * @param labels label image [const Array::Pointer &]
 * @param dst Parametric image computed[Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'label measurement', 'map', 'in assistant', 'combine'
 * @see https://clij.github.io/clij2-docs/reference_meanIntensityMap
 */
auto
mean_intensity_map_func(const Device::Pointer & device,
                        const Array::Pointer &  src,
                        const Array::Pointer &  labels,
                        Array::Pointer          dst) -> Array::Pointer;


/**
 * @name pixel_count_map
 * @brief Takes a label map, determines the number of pixels per label and replaces every label with the that number.
 * This results in a parametric image expressing area or volume.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Label image to measure [const Array::Pointer &]
 * @param dst Parametric image computed[Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'label measurement', 'map', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_pixelCountMap
 */
auto
pixel_count_map_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

/**
 * @name label_pixel_count_map
 * @brief Takes a label map, determines the number of pixels per label and replaces every label with the that number.
 * This results in a parametric image expressing area or volume.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Label image to measure [const Array::Pointer &]
 * @param dst Parametric image computed[Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'label measurement', 'map', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_pixelCountMap
 * @deprecated This function is deprecated. Use pixel_count_map_func instead.
 */
auto
label_pixel_count_map_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name centroids_of_labels
 * @brief Determines the centroids of all labels in a label image or image stack.
 * It writes the resulting coordinates in point list image of dimensions n * d
 * where n is the number of labels and d=3 the dimensionality (x,y,z) of the original image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param label_image Label image where the centroids will be determined from. [const Array::Pointer &]
 * @param centroids_coordinates Output list of coordinates where the centroids will be written to. [Array::Pointer ( = None )]
 * @param include_background Determines if the background label should be included. [bool ( = False )]
 * @return Array::Pointer
 *
 * @see https://clij.github.io/clij2-docs/reference_centroidsOfLabels
 */
auto
centroids_of_labels_func(const Device::Pointer & device,
                         const Array::Pointer &  label_image,
                         Array::Pointer          centroids_coordinates,
                         bool                    include_background) -> Array::Pointer;


/**
 * @name remove_labels_with_map_values_out_of_range
 * @brief Remove labels with values outside a given value range based on a vector of values
 * associated with the labels.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image where labels will be filtered. [const Array::Pointer &]
 * @param values Vector of values associated with the labels. [const Array::Pointer &]
 * @param dst Output image where labels will be written to. [Array::Pointer ( = None )]
 * @param min_value Minimum value to keep. [float ( = 0 )]
 * @param max_value Maximum value to keep. [float ( = 100 )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'combine'
 * @see https://clij.github.io/clij2-docs/reference_excludeLabelsWithValuesOutOfRange
 */
auto
remove_labels_with_map_values_out_of_range_func(const Device::Pointer & device,
                                                const Array::Pointer &  src,
                                                const Array::Pointer &  values,
                                                Array::Pointer          dst,
                                                float                   min_value,
                                                float                   max_value) -> Array::Pointer;

/**
 * @name remove_labels_with_map_values_within_range
 * @brief Remove labels with values inside a given value range based on a vector of values
 * associated with the labels.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image where labels will be filtered. [const Array::Pointer &]
 * @param values Vector of values associated with the labels. [const Array::Pointer &]
 * @param dst Output image where labels will be written to. [Array::Pointer ( = None )]
 * @param min_value Minimum value to keep. [float ( = 0 )]
 * @param max_value Maximum value to keep. [float ( = 100 )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'combine'
 * @see https://clij.github.io/clij2-docs/reference_excludeLabelsWithValuesWithinRange
 */
auto
remove_labels_with_map_values_within_range_func(const Device::Pointer & device,
                                                const Array::Pointer &  src,
                                                const Array::Pointer &  values,
                                                Array::Pointer          dst,
                                                float                   min_value,
                                                float                   max_value) -> Array::Pointer;

/**
 * @name exclude_labels_with_map_values_out_of_range
 * @brief Exclude labels with values outside a given value range based on a vector of values
 * associated with the labels.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param values_map Vector of values associated with the labels. [const Array::Pointer &]
 * @param label_map_input Input image where labels will be filtered. [const Array::Pointer &]
 * @param dst Output image where labels will be written to. [Array::Pointer ( = None )]
 * @param minimum_value_range Minimum value to keep. [float ( = 0 )]
 * @param maximum_value_range Maximum value to keep. [float ( = 100 )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'combine'
 * @see https://clij.github.io/clij2-docs/reference_excludeLabelsWithValuesOutOfRange
 * @deprecated This function is deprecated. Use remove_labels_with_map_values_out_of_range_func instead.
 */
auto
exclude_labels_with_map_values_out_of_range_func(const Device::Pointer & device,
                                                 const Array::Pointer &  values_map,
                                                 const Array::Pointer &  label_map_input,
                                                 Array::Pointer          dst,
                                                 float                   minimum_value_range,
                                                 float                   maximum_value_range) -> Array::Pointer;

/**
 * @name exclude_labels_with_map_values_within_range
 * @brief Exclude labels with values inside a given value range based on a vector of values
 * associated with the labels.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param values_map Vector of values associated with the labels. [const Array::Pointer &]
 * @param label_map_input Input image where labels will be filtered. [const Array::Pointer &]
 * @param dst Output image where labels will be written to. [Array::Pointer ( = None )]
 * @param minimum_value_range Minimum value to keep. [float ( = 0 )]
 * @param maximum_value_range Maximum value to keep. [float ( = 100 )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'combine'
 * @see https://clij.github.io/clij2-docs/reference_excludeLabelsWithValuesWithinRange
 * @deprecated This function is deprecated. Use remove_labels_with_values_within_range_func instead.
 */
auto
exclude_labels_with_map_values_within_range_func(const Device::Pointer & device,
                                                 const Array::Pointer &  values_map,
                                                 const Array::Pointer &  label_map_input,
                                                 Array::Pointer          dst,
                                                 float                   minimum_value_range,
                                                 float                   maximum_value_range) -> Array::Pointer;

/**
 * @name extension_ratio_map
 * @brief Determines the ratio of the extension for every label in a label map and returns it as
 * a parametric map.
 *
 * The extension ration is defined as the maximum distance of any pixel in the label to the label's centroid divided by
 * the average distance of all pixels in the label to the centroid.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input label image. [const Array::Pointer &]
 * @param dst Output parametric image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant', 'map'
 * @see https://clij.github.io/clij2-docs/reference_extensionRatioMap
 */
auto
extension_ratio_map_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


} // namespace cle::tier4

#endif // __INCLUDE_TIER4_HPP
