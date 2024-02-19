#ifndef __INCLUDE_TIER2_HPP
#define __INCLUDE_TIER2_HPP

#include "tier0.hpp"

/**
 * @namespace cle::tier2
 * @brief Namespace container for all functions of tier 2 category
 *        Tier 2 functions are advanced functions that may rely on previous tier functions
 */
namespace cle::tier2
{
/**
 * @name absolute_difference
 * @category 'combine', 'in assistant', 'bia-bob-suggestion'
 * @brief Determines the absolute difference pixel by pixel between two images. <pre>f(x, y) = |x y| </pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 The input image to be subtracted from. [const Array::Pointer &]
 * @param src1 The input image which is subtracted. [const Array::Pointer &]
 * @param dst The output image where results are written into. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @link https://clij.github.io/clij2-docs/reference_absoluteDifference
 */
auto
absolute_difference_func(const Device::Pointer & device,
                         const Array::Pointer &  src0,
                         const Array::Pointer &  src1,
                         Array::Pointer          dst) -> Array::Pointer;


/**
 * @name add_images
 * @priority -1
 * @category 'combine', 'in assistant'
 * @brief Calculates the sum of pairs of pixels x and y of two images X and Y. <pre>f(x, y) = x + y</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 The first input image to added. [const Array::Pointer &]
 * @param src1 The second image to be added. [const Array::Pointer &]
 * @param dst The output image where results are written into. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @link https://clij.github.io/clij2-docs/reference_addImages
 */
auto
add_images_func(const Device::Pointer & device,
                const Array::Pointer &  src0,
                const Array::Pointer &  src1,
                Array::Pointer          dst) -> Array::Pointer;


/**
 * @name bottom_hat_box
 * @priority -1
 * @category 'filter', 'background removal', 'in assistant'
 * @brief Apply a bottomhat filter for background subtraction to the input image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src The input image where the background is subtracted from. [const Array::Pointer &]
 * @param dst The output image where results are written into. [Array::Pointer ( = None )]
 * @param radius_x Radius of the background determination region in X. [int ( = 1 )]
 * @param radius_y Radius of the background determination region in Y. [int ( = 1 )]
 * @param radius_z Radius of the background determination region in Z. [int ( = 1 )]
 * @return Array::Pointer
 *
 * @link https://clij.github.io/clij2-docs/reference_bottomHatBox
 */
auto
bottom_hat_box_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    int                     radius_x,
                    int                     radius_y,
                    int                     radius_z) -> Array::Pointer;


/**
 * @name bottom_hat_sphere
 * @priority -1
 * @category 'filter', 'background removal', 'in assistant', 'bia-bob-suggestion'
 * @brief Applies a bottomhat filter for background subtraction to the input image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src The input image where the background is subtracted from. [const Array::Pointer &]
 * @param dst The output image where results are written into. [Array::Pointer ( = None )]
 * @param radius_x Radius of the background determination region in X. [float ( = 1 )]
 * @param radius_y Radius of the background determination region in Y. [float ( = 1 )]
 * @param radius_z Radius of the background determination region in Z. [float ( = 1 )]
 * @return Array::Pointer
 *
 * @link https://clij.github.io/clij2-docs/reference_bottomHatSphere
 */
auto
bottom_hat_sphere_func(const Device::Pointer & device,
                       const Array::Pointer &  src,
                       Array::Pointer          dst,
                       float                   radius_x,
                       float                   radius_y,
                       float                   radius_z) -> Array::Pointer;


/**
 * @name clip
 * @priority -1
 * @category 'combine', 'in assistant'
 * @brief Limits the range of values in an image. This function is supposed to work similarly as its counter part in
 * numpy [1].
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @param min_intensity new, lower limit of the intensity range [float ( = None )]
 * @param max_intensity new, upper limit of the intensity range [float ( = None )]
 * @return Array::Pointer
 *
 * @link https://numpy.org/doc/stable/reference/generated/numpy.clip.html
 */
auto
clip_func(const Device::Pointer & device,
          const Array::Pointer &  src,
          Array::Pointer          dst,
          float                   min_intensity,
          float                   max_intensity) -> Array::Pointer;


/**
 * @name closing_box
 * @category 'filter', 'in assistant'
 * @brief Closing operator, boxshaped Applies morphological closing to intensity images using a boxshaped footprint.
 * This operator also works with binary images.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @param radius_x [int ( = 0 )]
 * @param radius_y [int ( = 0 )]
 * @param radius_z [int ( = 0 )]
 * @return Array::Pointer
 *
 */
auto
closing_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 int                     radius_x,
                 int                     radius_y,
                 int                     radius_z) -> Array::Pointer;


/**
 * @name closing_sphere
 * @category 'filter', 'in assistant', 'bia-bob-suggestion'
 * @brief Closing operator, sphereshaped Applies morphological closing to intensity images using a sphereshaped
 * footprint. This operator also works with binary images.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @param radius_x [float ( = 1 )]
 * @param radius_y [float ( = 1 )]
 * @param radius_z [float ( = 0 )]
 * @return Array::Pointer
 *
 */
auto
closing_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z) -> Array::Pointer;


/**
 * @name concatenate_along_x
 * @category 'combine', 'transform', 'in assistant', 'bia-bob-suggestion'
 * @brief Concatenate two images or stacks along the X axis.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 [const Array::Pointer &]
 * @param src1 [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @link https://clij.github.io/clij2-docs/reference_combineHorizontally
 */
auto
concatenate_along_x_func(const Device::Pointer & device,
                         const Array::Pointer &  src0,
                         const Array::Pointer &  src1,
                         Array::Pointer          dst) -> Array::Pointer;

/**
 * @name concatenate_along_y
 * @category 'combine', 'transform', 'in assistant', 'bia-bob-suggestion'
 * @brief Concatenate two images or stacks along the Y axis.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 [const Array::Pointer &]
 * @param src1 [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @link https://clij.github.io/clij2-docs/reference_combineVertically
 */
auto
concatenate_along_y_func(const Device::Pointer & device,
                         const Array::Pointer &  src0,
                         const Array::Pointer &  src1,
                         Array::Pointer          dst) -> Array::Pointer;

/**
 * @name concatenate_along_z
 * @category 'combine', 'transform', 'in assistant', 'bia-bob-suggestion'
 * @brief Concatenate two images or stacks along the Z axis.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 [const Array::Pointer &]
 * @param src1 [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @link https://clij.github.io/clij2-docs/reference_concatenateStacks
 */
auto
concatenate_along_z_func(const Device::Pointer & device,
                         const Array::Pointer &  src0,
                         const Array::Pointer &  src1,
                         Array::Pointer          dst) -> Array::Pointer;

/**
 * @name count_touching_neighbors
 * @category 'bia-bob-suggestion'
 * @brief Takes a touch matrix as input and delivers a vector with number of touching neighbors per label as a vector.
 * Note: Background is considered as something that can touch. To ignore touches with background, hand over a touch
 * matrix where the first column (index = 0) has been set to 0. Use set_column for that.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @param ignore_background [bool ( = True )]
 * @return Array::Pointer
 *
 * @link https://clij.github.io/clij2-docs/reference_countTouchingNeighbors
 */
auto
count_touching_neighbors_func(const Device::Pointer & device,
                              const Array::Pointer &  src,
                              Array::Pointer          dst,
                              bool                    ignore_background) -> Array::Pointer;


/**
 * @name crop_border
 * @brief Crops an image by removing the outer pixels, per default 1. Notes * To make sure the output image has the
 * right size, provide destination_image=None.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @param border_size [int ( = 1 )]
 * @return Array::Pointer
 *
 */
auto
crop_border_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int border_size)
  -> Array::Pointer;


/**
 * @name divide_by_gaussian_background
 * @category 'filter', 'background removal', 'in assistant', 'bia-bob-suggestion'
 * @brief Applies Gaussian blur to the input image and divides the original by the result.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @param sigma_x [float ( = 2 )]
 * @param sigma_y [float ( = 2 )]
 * @param sigma_z [float ( = 2 )]
 * @return Array::Pointer
 *
 * @link https://clij.github.io/clij2-docs/reference_divideByGaussianBackground
 */
auto
divide_by_gaussian_background_func(const Device::Pointer & device,
                                   const Array::Pointer &  src,
                                   Array::Pointer          dst,
                                   float                   sigma_x,
                                   float                   sigma_y,
                                   float                   sigma_z) -> Array::Pointer;


/**
 * @name degrees_to_radians
 * @brief Converts radians to degrees.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 */
auto
degrees_to_radians_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

/**
 * @name detect_maxima_box
 * @category 'binarize', 'in assistant'
 * @brief Detects local maxima in a given square/cubic neighborhood. Pixels in the resulting image are set to 1 if there
 * is no other pixel in a given radius which has a higher intensity, and to 0 otherwise.
 *
 * @param device Device to perform the operation on. [const Device::Pointer & ( = None )]
 * @param src Input image to process. [const Array::Pointer & ( = 0 )]
 * @param dst Output result image. [Array::Pointer ( = 0 )]
 * @param radius_x Radius of the region in X. [int ( = 0 )]
 * @param radius_y Radius of the region in Y. [int ( = 0 )]
 * @param radius_z Radius of the region in Z. [int ( = 0 )]
 * @return Array::Pointer
 *
 * @link https://clij.github.io/clij2-docs/reference_detectMaximaBox
 */
auto
detect_maxima_box_func(const Device::Pointer & device,
                       const Array::Pointer &  src,
                       Array::Pointer          dst,
                       int                     radius_x,
                       int                     radius_y,
                       int                     radius_z) -> Array::Pointer;


/**
 * @name detect_minima_box
 * @category 'binarize', 'in assistant'
 * @brief Detects local maxima in a given square/cubic neighborhood. Pixels in the resulting image are set to 1 if there
 * is no other pixel in a given radius which has a lower intensity, and to 0 otherwise.
 *
 * @param device Device to perform the operation on. [const Device::Pointer & ( = None )]
 * @param src Input image to process. [const Array::Pointer & ( = 0 )]
 * @param dst Output result image. [Array::Pointer ( = 0 )]
 * @param radius_x Radius of the region in X. [int ( = 0 )]
 * @param radius_y Radius of the region in Y. [int ( = 0 )]
 * @param radius_z Radius of the region in Z. [int ( = 0 )]
 * @return Array::Pointer
 *
 * @link https://clij.github.io/clij2-docs/reference_detectMaximaBox
 */
auto
detect_minima_box_func(const Device::Pointer & device,
                       const Array::Pointer &  src,
                       Array::Pointer          dst,
                       int                     radius_x,
                       int                     radius_y,
                       int                     radius_z) -> Array::Pointer;

/**
 * @name difference_of_gaussian
 * @category 'filter', 'background removal', 'bia-bob-suggestion'
 * @brief Applies Gaussian blur to the input image twice with different sigma values resulting in two images which are
 * then subtracted from each other. It is recommended to apply this operation to images of type Float (32 bit) as
 * results might be negative.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src The input image to be processed. [const Array::Pointer &]
 * @param dst The output image where results are written into. [Array::Pointer ( = None )]
 * @param sigma1_x Sigma of the first Gaussian filter in x [float ( = 2 )]
 * @param sigma1_y Sigma of the first Gaussian filter in y [float ( = 2 )]
 * @param sigma1_z Sigma of the first Gaussian filter in z [float ( = 2 )]
 * @param sigma2_x Sigma of the second Gaussian filter in x [float ( = 2 )]
 * @param sigma2_y Sigma of the second Gaussian filter in y [float ( = 2 )]
 * @param sigma2_z Sigma of the second Gaussian filter in z [float ( = 2 )]
 * @return Array::Pointer
 *
 * @link https://clij.github.io/clij2-docs/reference_differenceOfGaussian3D
 */
auto
difference_of_gaussian_func(const Device::Pointer & device,
                            const Array::Pointer &  src,
                            Array::Pointer          dst,
                            float                   sigma1_x,
                            float                   sigma1_y,
                            float                   sigma1_z,
                            float                   sigma2_x,
                            float                   sigma2_y,
                            float                   sigma2_z) -> Array::Pointer;


/**
 * @name extend_labeling_via_voronoi
 * @category 'label processing', 'in assistant', 'bia-bob-suggestion'
 * @brief Takes a label map image and dilates the regions using a octagon shape until they touch. The resulting label
 * map is written to the output.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @link https://clij.github.io/clij2-docs/reference_extendLabelingViaVoronoi
 */
auto
extend_labeling_via_voronoi_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name invert
 * @category 'filter'
 * @brief Computes the negative value of all pixels in a given image. It is recommended to convert images to 32bit float
 * before applying this operation. <pre>f(x) = x</pre> For binary images, use binaryNot.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @link https://clij.github.io/clij2-docs/reference_invert
 */
auto
invert_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name label_spots
 * @category 'label', 'in assistant', 'bia-bob-suggestion'
 * @brief Transforms a binary image with single pixles set to 1 to a labelled spots image. Transforms a spots image as
 * resulting from maximum/minimum detection in an image of the same size where every spot has a number 1, 2,... n.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @link https://clij.github.io/clij2-docs/reference_labelSpots
 */
auto
label_spots_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name large_hessian_eigenvalue
 * @priority -1
 * @category 'filter', 'in assistant'
 * @brief Determines the Hessian eigenvalues and returns the large eigenvalue image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 */
auto
large_hessian_eigenvalue_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name maximum_of_all_pixels
 * @brief Determines the maximum of all pixels in a given image. It will be stored in a new row of ImageJs Results table
 * in the column 'Max'.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer & ( = None )]
 * @return float
 *
 * @link https://clij.github.io/clij2-docs/reference_maximumOfAllPixels
 */
auto
maximum_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float;


/**
 * @name minimum_of_all_pixels
 * @brief Determines the minimum of all pixels in a given image. It will be stored in a new row of ImageJs Results table
 * in the column 'Min'.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer & ( = None )]
 * @return float
 *
 * @link https://clij.github.io/clij2-docs/reference_minimumOfAllPixels
 */
auto
minimum_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float;


/**
 * @name minimum_of_masked_pixels
 * @brief Determines the minimum intensity in a masked image. But only in pixels which have nonzero values in another
 * mask image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer & ( = None )]
 * @param mask [const Array::Pointer & ( = 1 )]
 * @return float
 *
 * @link https://clij.github.io/clij2-docs/reference_minimumOfMaskedPixels
 */
auto
minimum_of_masked_pixels_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & mask)
  -> float;


/**
 * @name opening_box
 * @category 'filter', 'in assistant'
 * @brief Opening operator, boxshaped Applies morphological opening to intensity images using a boxshaped footprint.
 * This operator also works with binary images.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @param radius_x [int ( = 0 )]
 * @param radius_y [int ( = 0 )]
 * @param radius_z [int ( = 0 )]
 * @return Array::Pointer
 *
 */
auto
opening_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 int                     radius_x,
                 int                     radius_y,
                 int                     radius_z) -> Array::Pointer;


/**
 * @name opening_sphere
 * @category 'filter', 'in assistant', 'bia-bob-suggestion'
 * @brief Opening operator, sphereshaped Applies morphological opening to intensity images using a sphereshaped
 * footprint. This operator also works with binary images.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @param radius_x [float ( = 1 )]
 * @param radius_y [float ( = 1 )]
 * @param radius_z [float ( = 0 )]
 * @return Array::Pointer
 *
 */
auto
opening_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z) -> Array::Pointer;


/**
 * @name radians_to_degrees
 * @brief Converts radians to degrees
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 */
auto
radians_to_degrees_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name small_hessian_eigenvalue
 * @priority -1
 * @category 'filter', 'in assistant'
 * @brief Determines the Hessian eigenvalues and returns the small eigenvalue image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 */
auto
small_hessian_eigenvalue_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name square
 * @category 'filter'
 * @brief Return the elementwise square of the input. This function is supposed to be similar to its counterpart in
 * numpy [1]
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @link https://numpy.org/doc/stable/reference/generated/numpy.square.html
 */
auto
square_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name squared_difference
 * @category 'combine', 'in assistant', 'bia-bob-suggestion'
 * @brief Determines the squared difference pixel by pixel between two images.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 [const Array::Pointer &]
 * @param src1 [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @link https://clij.github.io/clij2-docs/reference_squaredDifference
 */
auto
squared_difference_func(const Device::Pointer & device,
                        const Array::Pointer &  src0,
                        const Array::Pointer &  src1,
                        Array::Pointer          dst) -> Array::Pointer;


/**
 * @name standard_deviation_box
 * @category 'filter', 'edge detection', 'in assistant'
 * @brief Computes the local standard deviation of a pixels box neighborhood. The box size is specified by its
 * halfwidth, halfheight and halfdepth (radius). If 2D images are given, radius_z will be ignored.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @param radius_x [int ( = 1 )]
 * @param radius_y [int ( = 1 )]
 * @param radius_z [int ( = 1 )]
 * @return Array::Pointer
 *
 * @link https://clij.github.io/clij2-docs/reference_standardDeviationBox
 */
auto
standard_deviation_box_func(const Device::Pointer & device,
                            const Array::Pointer &  src,
                            Array::Pointer          dst,
                            int                     radius_x,
                            int                     radius_y,
                            int                     radius_z) -> Array::Pointer;


/**
 * @name standard_deviation_sphere
 * @category 'filter', 'edge detection', 'in assistant', 'bia-bob-suggestion'
 * @brief Computes the local standard deviation of a pixels sphere neighborhood. The box size is specified by its
 * halfwidth, halfheight and halfdepth (radius). If 2D images are given, radius_z will be ignored.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @param radius_x [int ( = 1 )]
 * @param radius_y [int ( = 1 )]
 * @param radius_z [int ( = 1 )]
 * @return Array::Pointer
 *
 * @link https://clij.github.io/clij2-docs/reference_standardDeviationSphere
 */
auto
standard_deviation_sphere_func(const Device::Pointer & device,
                               const Array::Pointer &  src,
                               Array::Pointer          dst,
                               int                     radius_x,
                               int                     radius_y,
                               int                     radius_z) -> Array::Pointer;


/**
 * @name subtract_gaussian_background
 * @category 'filter', 'background removal', 'in assistant', 'bia-bob-suggestion'
 * @brief Applies Gaussian blur to the input image and subtracts the result from the original.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @param sigma_x [float ( = 2 )]
 * @param sigma_y [float ( = 2 )]
 * @param sigma_z [float ( = 2 )]
 * @return Array::Pointer
 *
 * @link https://clij.github.io/clij2-docs/reference_subtractGaussianBackground
 */
auto
subtract_gaussian_background_func(const Device::Pointer & device,
                                  const Array::Pointer &  src,
                                  Array::Pointer          dst,
                                  float                   sigma_x,
                                  float                   sigma_y,
                                  float                   sigma_z) -> Array::Pointer;


/**
 * @name subtract_images
 * @priority -1
 * @category 'combine', 'in assistant'
 * @brief Subtracts one image X from another image Y pixel wise. <pre>f(x, y) = x y</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 [const Array::Pointer &]
 * @param src1 [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @link https://clij.github.io/clij2-docs/reference_subtractImages
 */
auto
subtract_images_func(const Device::Pointer & device,
                     const Array::Pointer &  src0,
                     const Array::Pointer &  src1,
                     Array::Pointer          dst) -> Array::Pointer;


/**
 * @name sum_of_all_pixels
 * @brief Determines the sum of all pixels in a given image. It will be stored in a new row of ImageJs Results table in
 * the column 'Sum'.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src [const Array::Pointer & ( = None )]
 * @return float
 *
 * @link https://clij.github.io/clij2-docs/reference_sumOfAllPixels
 */
auto
sum_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float;


/**
 * @name top_hat_box
 * @priority 1
 * @category 'filter', 'background removal', 'in assistant'
 * @brief Applies a tophat filter for background subtraction to the input image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src The input image where the background is subtracted from. [const Array::Pointer &]
 * @param dst The output image where results are written into. [Array::Pointer ( = None )]
 * @param radius_x Radius of the background determination region in X. [int ( = 1 )]
 * @param radius_y Radius of the background determination region in Y. [int ( = 1 )]
 * @param radius_z Radius of the background determination region in Z. [int ( = 1 )]
 * @return Array::Pointer
 *
 * @link https://clij.github.io/clij2-docs/reference_topHatBox
 */
auto
top_hat_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 int                     radius_x,
                 int                     radius_y,
                 int                     radius_z) -> Array::Pointer;


/**
 * @name top_hat_sphere
 * @category 'filter', 'background removal', 'in assistant', 'bia-bob-suggestion'
 * @brief Applies a tophat filter for background subtraction to the input image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src The input image where the background is subtracted from. [const Array::Pointer &]
 * @param dst The output image where results are written into. [Array::Pointer ( = None )]
 * @param radius_x Radius of the background determination region in X. [float ( = 1 )]
 * @param radius_y Radius of the background determination region in Y. [float ( = 1 )]
 * @param radius_z Radius of the background determination region in Z. [float ( = 1 )]
 * @return Array::Pointer
 *
 * @link https://clij.github.io/clij2-docs/reference_topHatSphere
 */
auto
top_hat_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z) -> Array::Pointer;


} // namespace cle::tier2

#endif // __INCLUDE_TIER2_HPP
