#ifndef __INCLUDE_TIER3_HPP
#define __INCLUDE_TIER3_HPP

#include "statistics.hpp"
#include "tier0.hpp"

/**
 * @namespace cle::tier3
 * @brief Namespace container for all functions of tier 3 category
 *        Tier 3 functions are advanced functions that may rely on previous tier functions
 */
namespace cle::tier3
{
/**
 * @name bounding_box
 * @brief Determines the bounding box of all nonzero pixels in a binary image. The positions are returned in
 *  an array of 6 values as follows: minX, minY, minZ, maxX, maxY, maxZ.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input binary image [const Array::Pointer &]
 * @return std::vector<float>
 *
 * @see https://clij.github.io/clij2-docs/reference_boundingBox
 *
 */
auto
bounding_box_func(const Device::Pointer & device, const Array::Pointer & src) -> std::vector<float>;


/**
 * @name center_of_mass
 * @brief Determines the center of mass of an image or image stack. It writes the result in the results table in the
 * columns MassX, MassY and MassZ.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image [const Array::Pointer &]
 * @return std::vector<float>
 * @see https://clij.github.io/clij2-docs/reference_centerOfMass
 *
 */
auto
center_of_mass_func(const Device::Pointer & device, const Array::Pointer & src) -> std::vector<float>;

/**
 * @name clahe
 * @brief Applies CLAHE (Contrast Limited Adaptive Histogram Equalization) to the input image. The algorithm is
 * adapted from the work of Hugo Raveton (https://github.com/HugoRaveton/pyopencl_clahe)
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param tile_size Size of the tiles to be used for CLAHE. [int ( = 8 )]
 * @param clip_limit Clip limit for CLAHE. [float ( = 0.01 )]
 * @param minimum_intensity Minimum intensity value. [float ( = float('nan') )]
 * @param maximum_intensity Maximum intensity value. [float ( = float('nan') )]
 * @return Array::Pointer
 *
 */
auto
clahe_func(const Device::Pointer & device,
           const Array::Pointer &  src,
           Array::Pointer          dst,
           int                     tile_size,
           float                   clip_limit,
           float                   minimum_intensity = NaN,
           float                   maximum_intensity = NaN) -> Array::Pointer;

/**
 * @name remove_labels
 * @brief This operation removes labels from a labelmap and renumbers the remaining labels. Hand over a binary flag list
 * vector starting with a flag for the background, continuing with label1, label2,... For example if you pass 0,1,0,0,1:
 * Labels 1 and 4 will be removed (those with a 1 in the vector will be excluded). Labels 2 and 3 will be kept and
 * renumbered to 1 and 2.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input label image [const Array::Pointer &]
 * @param list Vector of 0 and 1 flagging labels to remove [const Array::Pointer &]
 * @param dst Output label image [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @see https://clij.github.io/clij2-docs/reference_excludeLabels
 *
 */
auto
remove_labels_func(const Device::Pointer & device,
                   const Array::Pointer &  src,
                   const Array::Pointer &  list,
                   Array::Pointer          dst) -> Array::Pointer;

/**
 * @name exclude_labels
 * @brief This operation removes labels from a labelmap and renumbers the remaining labels. Hand over a binary flag list
 * vector starting with a flag for the background, continuing with label1, label2,... For example if you pass 0,1,0,0,1:
 * Labels 1 and 4 will be removed (those with a 1 in the vector will be excluded). Labels 2 and 3 will be kept and
 * renumbered to 1 and 2.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input label image [const Array::Pointer &]
 * @param list Vector of 0 and 1 flagging labels to remove [const Array::Pointer &]
 * @param dst Output label image [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @see https://clij.github.io/clij2-docs/reference_excludeLabels
 *
 */
auto
exclude_labels_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    const Array::Pointer &  list,
                    Array::Pointer          dst) -> Array::Pointer;

/**
 * @name remove_labels_on_edges
 * @brief Removes all labels from a label map which touch the edges of the image. Remaining label elements are
 * renumbered afterwards.
 *
 * @param device Device to perform the operation on. [const Device::Pointer & ( = None )]
 * @param src Input label image [const Array::Pointer &]
 * @param dst Output label image[Array::Pointer ( = None )]
 * @param exclude_x Exclude labels along min and max x [bool ( = True )]
 * @param exclude_y Exclude labels along min and max y [bool ( = True )]
 * @param exclude_z Exclude labels along min and max z [bool ( = True )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_excludeLabelsOnEdges
 */
auto
remove_labels_on_edges_func(const Device::Pointer & device,
                            const Array::Pointer &  src,
                            Array::Pointer          dst,
                            bool                    exclude_x,
                            bool                    exclude_y,
                            bool                    exclude_z) -> Array::Pointer;

/**
 * @name exclude_labels_on_edges
 * @brief Removes all labels from a label map which touch the edges of the image. Remaining label elements are
 * renumbered afterwards.
 *
 * @param device Device to perform the operation on. [const Device::Pointer & ( = None )]
 * @param src Input label image [const Array::Pointer &]
 * @param dst Output label image [Array::Pointer ( = None )]
 * @param exclude_x Exclude labels along min and max x [bool ( = True )]
 * @param exclude_y Exclude labels along min and max y [bool ( = True )]
 * @param exclude_z Exclude labels along min and max z [bool ( = True )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_excludeLabelsOnEdges
 */
auto
exclude_labels_on_edges_func(const Device::Pointer & device,
                             const Array::Pointer &  src,
                             Array::Pointer          dst,
                             bool                    exclude_x,
                             bool                    exclude_y,
                             bool                    exclude_z) -> Array::Pointer;

/**
 * @name flag_existing_labels
 * @brief Given a label map this function will generate a binary vector where all pixels are set to 1 if label with
 * given xcoordinate in the vector exists. For example a label image such as ``` 0 1 3 5 ``` will produce a flag_vector
 * like this: ``` 1 1 0 1 0 1 ```
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src a label image [const Array::Pointer &]
 * @param dst binary vector, if given should have size 1*n with n = maximum label + 1 [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 */
auto
flag_existing_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name gamma_correction
 * @brief Applies a gamma correction to an image. Therefore, all pixels x of the Image X are normalized and the power to
 * gamma g is computed, before normlization is reversed (^ is the power operator):f(x) = (x / max(X)) ^ gamma * max(X)
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image [const Array::Pointer &]
 * @param dst Output image [Array::Pointer ( = None )]
 * @param gamma [float ( = 1 )]
 * @return Array::Pointer
 *
 * @note 'filter', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_gammaCorrection
 */
auto
gamma_correction_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float gamma)
  -> Array::Pointer;


/**
 * @name generate_binary_overlap_matrix
 * @brief Takes two labelmaps with n and m labels and generates a (n+1)*(m+1) matrix where all pixels are set to 0 exept
 * those where labels overlap between the label maps. For example, if labels 3 in labelmap1 and 4 in labelmap2 are
 * touching then the pixel (3,4) in the matrix will be set to 1.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 First input label image [const Array::Pointer &]
 * @param src1 Second input label image [const Array::Pointer &]
 * @param dst Output overlap matrix [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @see https://clij.github.io/clij2-docs/reference_generateBinaryOverlapMatrix
 *
 */
auto
generate_binary_overlap_matrix_func(const Device::Pointer & device,
                                    const Array::Pointer &  src0,
                                    const Array::Pointer &  src1,
                                    Array::Pointer          dst) -> Array::Pointer;


/**
 * @name generate_touch_matrix
 * @brief Takes a labelmap with n labels and generates a (n+1)*(n+1) matrix where all pixels are set to 0 exept those
 * where labels are touching. Only half of the matrix is filled (with x < y). For example, if labels 3 and 4 are
 * touching then the pixel (3,4) in the matrix will be set to 1. The touch matrix is a representation of a region
 * adjacency graph
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input label image [const Array::Pointer &]
 * @param dst_matrix Output touch matrix [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @see https://clij.github.io/clij2-docs/reference_generateTouchMatrix
 */
auto
generate_touch_matrix_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst_matrix)
  -> Array::Pointer;


/**
 * @name histogram
 * @brief Determines the histogram of a given image. The histogram image is of dimensions number_of_bins/1/1; a 3D image
 * with height=1 and depth=1. Histogram bins contain the number of pixels with intensity in this corresponding bin. The
 * histogram bins are uniformly distributed between given minimum and maximum grey value intensity. If the flag
 * determine_min_max is set, minimum and maximum intensity will be determined. When calling this operation many times,
 * it is recommended to determine minimum and maximum intensity once at the beginning and handing over these values.
 * Author(s): Robert Haase adapted work from Aaftab Munshi, Benedict Gaster, Timothy Mattson, James Fung, Dan Ginsburg
 * License: adapted code from
 * https://github.com/bgaster/openclbooksamples/blob/master/src/Chapter_14/histogram/histogram_image.cl
 *
 * It was published unter BSD license according to
 * https://code.google.com/archive/p/openclbooksamples/
 *
 * Book: OpenCL(R) Programming Guide
 * Authors: Aaftab Munshi, Benedict Gaster, Timothy Mattson, James Fung, Dan Ginsburg
 * ISBN10: 0321749642
 * ISBN13: 9780321749642
 * Publisher: AddisonWesley Professional
 * URLs: http://safari.informit.com/9780132488006/
 * http://www.openclprogrammingguide.com
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to derive histogram from [const Array::Pointer &]
 * @param dst Output histogram [Array::Pointer ( = None )]
 * @param num_bins [int ( = 256 )]
 * @param minimum_intensity [float ( = float('nan') )]
 * @param maximum_intensity [float ( = float('nan') )]
 * @return Array::Pointer
 *
 * @see https://clij.github.io/clij2-docs/reference_histogram
 *
 */
auto
histogram_func(const Device::Pointer & device,
               const Array::Pointer &  src,
               Array::Pointer          dst,
               int                     num_bins,
               float                   minimum_intensity = NaN,
               float                   maximum_intensity = NaN) -> Array::Pointer;


/**
 * @name jaccard_index
 * @brief Determines the overlap of two binary images using the Jaccard index. A value of 0 suggests no overlap, 1 means
 * perfect overlap. The resulting Jaccard index is saved to the results table in the 'Jaccard_Index' column. Note that
 * the SorensenDice coefficient can be calculated from the Jaccard index j using this formula: <pre>s = f(j) = 2 j / (j
 * + 1)</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 First binary image to compare [const Array::Pointer &]
 * @param src1 Second binary image to compare [const Array::Pointer &]
 * @return float
 *
 * @see https://clij.github.io/clij2-docs/reference_jaccardIndex
 *
 */
auto
jaccard_index_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1) -> float;


/**
 * @name labelled_spots_to_pointlist
 * @brief Generates a coordinate list of points in a labelled spot image. Transforms a labelmap of spots (single pixels
 * with values 1, 2,..., n for n spots) as resulting from connected components analysis in an image where every column
 * contains d pixels (with d = dimensionality of the original image) with the coordinates of the maxima/minima.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param label Input label image [const Array::Pointer &]
 * @param pointlist Output coordinate list [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @see https://clij.github.io/clij2-docs/reference_labelledSpotsToPointList
 */
auto
labelled_spots_to_pointlist_func(const Device::Pointer & device,
                                 const Array::Pointer &  label,
                                 Array::Pointer          dspointlistt) -> Array::Pointer;


/**
 * @name maximum_position
 * @brief Determines the position of the maximum of all pixels in a given image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src The image of which the position of the maximum of all pixels will be determined. [const Array::Pointer &]
 * @return std::vector<float>
 *
 */
auto
maximum_position_func(const Device::Pointer & device, const Array::Pointer & src) -> std::vector<float>;


/**
 * @name mean_of_all_pixels
 * @brief Determines the mean average of all pixels in a given image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src The image of which the mean average of all pixels will be determined. [const Array::Pointer &]
 * @return float
 * @see https://clij.github.io/clij2-docs/reference_meanOfAllPixels
 *
 */
auto
mean_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float;


/**
 * @name minimum_position
 * @brief Determines the position of the minimum of all pixels in a given image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src  The image of which the position of the minimum of all pixels will be determined. [const Array::Pointer &]
 * @return std::vector<float>
 *
 */
auto
minimum_position_func(const Device::Pointer & device, const Array::Pointer & src) -> std::vector<float>;


/**
 * @name morphological_chan_vese
 * @brief Compute an active contour model using the Chan-Vese morphological algorithm. The output image (dst) should
 * also be initialisation of the contour. If not provided (nullptr), the function will use a checkboard pattern
 * initialisation.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output contour, can also be use to provide initialisation. [Array::Pointer ( = None )]
 * @param num_iter Number of iterations. [int ( = 100 )]
 * @param smoothing Number of smoothing iterations. [int ( = 1 )]
 * @param lambda1 Lambda1. [float ( = 1 )]
 * @param lambda2 Lambda2. [float ( = 1 )]
 * @return Array::Pointer
 */
auto
morphological_chan_vese_func(const Device::Pointer & device,
                             const Array::Pointer &  src,
                             Array::Pointer          dst,
                             int                     num_iter,
                             int                     smoothing,
                             float                   lambda1,
                             float                   lambda2) -> Array::Pointer;

/**
 * @name statistics_of_labelled_pixels
 * @brief Compute the bounding box, area (in pixels/voxels), minimum intensity, maximum intensity, average intensity,
 * standard deviation of the intensity, and some shape descriptors of labelled objects in a label image and its
 * corresponding intensity image.
 *
 * The intensity image is equal to the label image if not provided.
 * The label image is set to the entire image if not provided.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param intensity Intensity image. [Array::Pointer ( = None )]
 * @param label Label image to compute the statistics. [Array::Pointer ( = None )]]
 * @return StatisticsMap
 *
 * @see https://clij.github.io/clij2-docs/reference_statisticsOfLabelledPixels
 */
auto
statistics_of_labelled_pixels_func(const Device::Pointer & device, Array::Pointer intensity, Array::Pointer label)
  -> StatisticsMap;

/**
 * @name statistics_of_background_and_labelled_pixels
 * @brief Compute, for the background and labels, the bounding box, area (in pixels/voxels), minimum intensity,
 * maximum intensity, average intensity, standard deviation of the intensity, and some shape descriptors of
 * labelled objects in a label image and its corresponding intensity image.
 *
 * The intensity image is equal to the label image if not provided.
 * The label image is set to the entire image if not provided.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param intensity Intensity image. [Array::Pointer ( = None )]
 * @param label Label image to compute the statistics. [Array::Pointer ( = None )]]
 * @return StatisticsMap
 *
 * @see https://clij.github.io/clij2-docs/reference_statisticsOfBackgroundAndLabelledPixels
 */
auto
statistics_of_background_and_labelled_pixels_func(const Device::Pointer & device,
                                                  Array::Pointer          intensity,
                                                  Array::Pointer          label) -> StatisticsMap;

/**
 * @name sato_filter
 * @brief Applies the multi-scale ridge detection Sato filter.
 * This filter was first introduced by Sato et al. in 1998 (https://doi.org/10.1016/S1361-8415(98)80009-1)
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param sigma_minimum Minimum sigma value for the filter. [float ( = 1 )]
 * @param sigma_maximum Maximum sigma value for the filter. [float ( = 3 )]
 * @param sigma_step Step size for the sigma values. [float ( = 1 )]
 * @return Array::Pointer
 *
 * @note 'filter', 'in assistant'
 * @see https://doi.org/10.1016/S1361-8415(98)80009-1
 */
auto
sato_filter_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 float                   sigma_minimum,
                 float                   sigma_maximum,
                 float                   sigma_step) -> Array::Pointer;

                 
                 
  /**
  * @name tubeness
  * @brief Enhances filamentous structures of a specified thickness in 2D or 3D
  * This function is a reimplementation of the Tubeness filter from Fiji/ImageJ
  * 
  * @param device Device to perform the operation on. [const Device::Pointer &]
  * @param src Input image to process. [const Array::Pointer &]
  * @param dst Output result image. [Array::Pointer ( = None )]
  * @param sigma Standard deviation of the Gaussian kernel used in the filter. [float ( = 1 )]
  * @return Array::Pointer
  * 
  * @note 'filter', 'in assistant'
  * @see https://imagej.net/plugins/tubeness
  */
  auto
  tubeness_func(const Device::Pointer & device,
  const Array::Pointer &  src,
  Array::Pointer          dst,
  float                   sigma) -> Array::Pointer;
  
} // namespace cle::tier3

#endif // __INCLUDE_TIER3_HPP
