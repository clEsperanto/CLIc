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
 * @param src [const Array::Pointer &]
 * @return std::array<float, 6>
 *
 * @see https://clij.github.io/clij2-docs/reference_boundingBox
 *
 */
auto
bounding_box_func(const Device::Pointer & device, const Array::Pointer & src) -> std::array<float, 6>;


/**
 * @name center_of_mass
 * @brief Determines the center of mass of an image or image stack. It writes the result in the results table in the
 * columns MassX, MassY and MassZ.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @return std::array<float, 3>
 * @see https://clij.github.io/clij2-docs/reference_centerOfMass
 *
 */
auto
center_of_mass_func(const Device::Pointer & device, const Array::Pointer & src) -> std::array<float, 3>;


/**
 * @name exclude_labels
 * @brief This operation removes labels from a labelmap and renumbers the remaining labels. Hand over a binary flag list
 * vector starting with a flag for the background, continuing with label1, label2,... For example if you pass 0,1,0,0,1:
 * Labels 1 and 4 will be removed (those with a 1 in the vector will be excluded). Labels 2 and 3 will be kept and
 * renumbered to 1 and 2.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param list [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
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
 * @name exclude_labels_on_edges
 * @brief Removes all labels from a label map which touch the edges of the image. Remaining label elements are
 * renumbered afterwards.
 *
 * @param device Device to perform the operation on. [const Device::Pointer & ( = None )]
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @param exclude_x Exclude labels along min and max x [bool ( = True )]
 * @param exclude_y Exclude labels along min and max y [bool ( = True )]
 * @param exclude_z Exclude labels along min and max z [bool ( = True )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant', 'bia-bob-suggestion'
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
flag_existing_labels_func(const Device::Pointer & device,
                          const Array::Pointer &  src,
                          Array::Pointer          dst) -> Array::Pointer;


/**
 * @name gamma_correction
 * @brief Applies a gamma correction to an image. Therefore, all pixels x of the Image X are normalized and the power to
 * gamma g is computed, before normlization is reversed (^ is the power operator):f(x) = (x / max(X)) ^ gamma * max(X)
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
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
 * @param src0 [const Array::Pointer &]
 * @param src1 [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
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
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_generateTouchMatrix
 */
auto
generate_touch_matrix_func(const Device::Pointer & device,
                           const Array::Pointer &  src,
                           Array::Pointer          dst) -> Array::Pointer;


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
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @param nbins [int ( = 256 )]
 * @param min [float ( = None )]
 * @param max [float ( = None )]
 * @return Array::Pointer
 *
 * @see https://clij.github.io/clij2-docs/reference_histogram
 *
 */
auto
histogram_func(const Device::Pointer & device,
               const Array::Pointer &  src,
               Array::Pointer          dst,
               int                     nbins,
               float                   min = NaN,
               float                   max = NaN) -> Array::Pointer;


/**
 * @name jaccard_index
 * @brief Determines the overlap of two binary images using the Jaccard index. A value of 0 suggests no overlap, 1 means
 * perfect overlap. The resulting Jaccard index is saved to the results table in the 'Jaccard_Index' column. Note that
 * the SorensenDice coefficient can be calculated from the Jaccard index j using this formula: <pre>s = f(j) = 2 j / (j
 * + 1)</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 [const Array::Pointer &]
 * @param src1 [const Array::Pointer &]
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
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_labelledSpotsToPointList
 */
auto
labelled_spots_to_pointlist_func(const Device::Pointer & device,
                                 const Array::Pointer &  src,
                                 Array::Pointer          dst) -> Array::Pointer;


/**
 * @name maximum_position
 * @brief Determines the position of the maximum of all pixels in a given image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src The image of which the position of the maximum of all pixels will be determined. [const Array::Pointer &]
 * @return std::array<size_t, 3>
 *
 */
auto
maximum_position_func(const Device::Pointer & device, const Array::Pointer & src) -> std::array<size_t, 3>;


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
 * @return std::array<size_t, 3>
 *
 */
auto
minimum_position_func(const Device::Pointer & device, const Array::Pointer & src) -> std::array<size_t, 3>;


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
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param input Input label image. [Array::Pointer]
 * @param intensity Input intensity image. [Array::Pointer ( = None )]
 * @return StatisticsMap
 */
auto
statistics_of_labelled_pixels(const Device::Pointer & device,
                              const Array::Pointer &  input,
                              Array::Pointer          intensity) -> StatisticsMap;

} // namespace cle::tier3

#endif // __INCLUDE_TIER3_HPP
