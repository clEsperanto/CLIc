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
 * @brief Determines the absolute difference pixel by pixel between two images. <pre>f(x, y) = |x y| </pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 The input image to be subtracted from. [const Array::Pointer &]
 * @param src1 The input image which is subtracted. [const Array::Pointer &]
 * @param dst The output image where results are written into. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'combine', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_absoluteDifference
 */
auto
absolute_difference_func(const Device::Pointer & device,
                         const Array::Pointer &  src0,
                         const Array::Pointer &  src1,
                         Array::Pointer          dst) -> Array::Pointer;


/**
 * @name add_images
 * @brief Calculates the sum of pairs of pixels x and y of two images X and Y. <pre>f(x, y) = x + y</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 The first input image to added. [const Array::Pointer &]
 * @param src1 The second image to be added. [const Array::Pointer &]
 * @param dst The output image where results are written into. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'combine', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_addImages
 */
auto
add_images_func(const Device::Pointer & device,
                const Array::Pointer &  src0,
                const Array::Pointer &  src1,
                Array::Pointer          dst) -> Array::Pointer;


/**
 * @name bottom_hat_box
 * @brief Apply a bottomhat filter for background subtraction to the input image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src The input image where the background is subtracted from. [const Array::Pointer &]
 * @param dst The output image where results are written into. [Array::Pointer ( = None )]
 * @param radius_x Radius of the background determination region in X. [float ( = 1 )]
 * @param radius_y Radius of the background determination region in Y. [float ( = 1 )]
 * @param radius_z Radius of the background determination region in Z. [float ( = 1 )]
 * @return Array::Pointer
 *
 * @note 'filter', 'background removal', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_bottomHatBox
 * @deprecated This method is deprecated. Consider using bottom_hat() instead.
 */
auto
bottom_hat_box_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z) -> Array::Pointer;


/**
 * @name bottom_hat_sphere
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
 * @note 'filter', 'background removal', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_bottomHatSphere
 * @deprecated This method is deprecated. Consider using bottom_hat() instead.
 */
auto
bottom_hat_sphere_func(const Device::Pointer & device,
                       const Array::Pointer &  src,
                       Array::Pointer          dst,
                       float                   radius_x,
                       float                   radius_y,
                       float                   radius_z) -> Array::Pointer;

/**
 * @name bottom_hat
 * @brief Applies a bottomhat filter for background subtraction to the input image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src The input image where the background is subtracted from. [const Array::Pointer &]
 * @param dst The output image where results are written into. [Array::Pointer ( = None )]
 * @param radius_x Radius of the background determination region in X. [float ( = 1 )]
 * @param radius_y Radius of the background determination region in Y. [float ( = 1 )]
 * @param radius_z Radius of the background determination region in Z. [float ( = 1 )]
 * @param connectivity Element shape, "box" or "sphere" [std::string ( = "box" )]
 * @return Array::Pointer
 *
 * @note 'filter', 'background removal', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_bottomHatBox
 * @see https://clij.github.io/clij2-docs/reference_bottomHatSphere
 */
auto
bottom_hat_func(const Device::Pointer & device,
                const Array::Pointer &  src,
                Array::Pointer          dst,
                float                   radius_x,
                float                   radius_y,
                float                   radius_z,
                std::string             connectivity) -> Array::Pointer;

/**
 * @name clip
 * @brief Limits the range of values in an image. This function is supposed to work similarly as its counter part in
 * numpy [1].
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param min_intensity new, lower limit of the intensity range [float ( = None )]
 * @param max_intensity new, upper limit of the intensity range [float ( = None )]
 * @return Array::Pointer
 *
 * @note 'combine', 'in assistant'
 * @see https://numpy.org/doc/stable/reference/generated/numpy.clip.html
 */
auto
clip_func(const Device::Pointer & device,
          const Array::Pointer &  src,
          Array::Pointer          dst,
          float                   min_intensity,
          float                   max_intensity) -> Array::Pointer;


/**
 * @name closing_box
 * @brief Closing operator, applies grayscale morphological closing to intensity images using a box shaped
 * footprint. This operator also works with binary images.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius along the x axis. [int ( = 1 )]
 * @param radius_y Radius along the y axis. [int ( = 1 )]
 * @param radius_z Radius along the z axis. [int ( = 1 )]
 * @return Array::Pointer
 * @note 'filter', 'in assistant'
 * @deprecated This method is deprecated. Consider using closing() instead.
 */
auto
closing_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 float                   radius_x,
                 float                   radius_y,
                 float                   radius_z) -> Array::Pointer;


/**
 * @name closing_sphere
 * @brief Closing operator, applies grayscale morphological closing to intensity images using a sphere shaped
 * footprint. This operator also works with binary images.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius along the x axis. [float ( = 1 )]
 * @param radius_y Radius along the y axis. [float ( = 1 )]
 * @param radius_z Radius along the z axis. [float ( = 1 )]
 * @return Array::Pointer
 * @note 'filter', 'in assistant', 'bia-bob-suggestion'
 * @deprecated This method is deprecated. Consider using closing() instead.
 */
auto
closing_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z) -> Array::Pointer;

/**
 * @name grayscale_closing
 * @brief Closing operator, applies grayscale morphological closing to intensity images using a sphere or box shaped
 * footprint. This operator also works with binary images.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius along the x axis. [float ( = 1 )]
 * @param radius_y Radius along the y axis. [float ( = 1 )]
 * @param radius_z Radius along the z axis. [float ( = 1 )]
 * @param connectivity Element shape, "box" or "sphere" [std::string ( = "box" )]
 * @return Array::Pointer
 * @note 'filter', 'in assistant'
 */
auto
grayscale_closing_func(const Device::Pointer & device,
                       const Array::Pointer &  src,
                       Array::Pointer          dst,
                       float                   radius_x,
                       float                   radius_y,
                       float                   radius_z,
                       std::string             connectivity) -> Array::Pointer;


/**
 * @name closing
 * @brief Closing operator, applies morphological closing to intensity images using a custom structuring element
 * provided as input. This operator also works with binary images.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param footprint Structuring element for the operation. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 * @note 'filter', 'in assistant'
 */
auto
closing_func(const Device::Pointer & device,
             const Array::Pointer &  src,
             const Array::Pointer &  footprint,
             Array::Pointer          dst) -> Array::Pointer;

/**
 * @name binary_closing
 * @brief Closing operator, applies binary morphological closing to intensity images using a sphere or box shaped
 * footprint. This operator also works with binary images.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius of the sphere or box element along the x axis. [float ( = 1 )]
 * @param radius_y Radius of the sphere or box element along the y axis. [float ( = 1 )]
 * @param radius_z Radius of the sphere or box element along the z axis. [float ( = 1 )]
 * @param connectivity Element shape, "box" or "sphere" [std::string ( = "box" )]
 * @return Array::Pointer
 * @note 'filter', 'in assistant'
 */
auto
binary_closing_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z,
                    std::string             connectivity) -> Array::Pointer;

/**
 * @name concatenate_along_x
 * @brief Concatenate two images or stacks along the X axis.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 First input image. [const Array::Pointer &]
 * @param src1 Second input image. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'combine', 'transform', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_combineHorizontally
 */
auto
concatenate_along_x_func(const Device::Pointer & device,
                         const Array::Pointer &  src0,
                         const Array::Pointer &  src1,
                         Array::Pointer          dst) -> Array::Pointer;

/**
 * @name concatenate_along_y
 * @brief Concatenate two images or stacks along the Y axis.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 First input image. [const Array::Pointer &]
 * @param src1 Second input image. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'combine', 'transform', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_combineVertically
 */
auto
concatenate_along_y_func(const Device::Pointer & device,
                         const Array::Pointer &  src0,
                         const Array::Pointer &  src1,
                         Array::Pointer          dst) -> Array::Pointer;

/**
 * @name concatenate_along_z
 * @brief Concatenate two images or stacks along the Z axis.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 First input image. [const Array::Pointer &]
 * @param src1 Second input image. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'combine', 'transform', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_concatenateStacks
 */
auto
concatenate_along_z_func(const Device::Pointer & device,
                         const Array::Pointer &  src0,
                         const Array::Pointer &  src1,
                         Array::Pointer          dst) -> Array::Pointer;

/**
 * @name count_touching_neighbors
 * @brief Takes a touch matrix as input and delivers a vector with number of touching neighbors per label as a vector.
 * Note: Background is considered as something that can touch. To ignore touches with background, hand over a touch
 * matrix where the first column (index = 0) has been set to 0. Use set_column for that.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param touch_matrix Input touch matrix to process. [const Array::Pointer &]
 * @param touching_neighbors_count_destination Output vector of touch count. [Array::Pointer ( = None )]
 * @param ignore_background [bool ( = True )]
 * @return Array::Pointer
 *
 * @see https://clij.github.io/clij2-docs/reference_countTouchingNeighbors
 */
auto
count_touching_neighbors_func(const Device::Pointer & device,
                              const Array::Pointer &  touch_matrix,
                              Array::Pointer          touching_neighbors_count_destination,
                              bool                    ignore_background) -> Array::Pointer;


/**
 * @name crop_border
 * @brief Crops an image by removing the outer pixels, per default 1. Notes * To make sure the output image has the
 * right size, provide destination_image=None.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param border_size Border size to crop. [int ( = 1 )]
 * @return Array::Pointer
 *
 */
auto
crop_border_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int border_size)
  -> Array::Pointer;


/**
 * @name divide_by_gaussian_background
 * @brief Applies Gaussian blur to the input image and divides the original by the result.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param sigma_x Gaussian sigma value along x. [float ( = 2 )]
 * @param sigma_y Gaussian sigma value along y. [float ( = 2 )]
 * @param sigma_z Gaussian sigma value along z. [float ( = 2 )]
 * @return Array::Pointer
 *
 * @note 'filter', 'background removal', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_divideByGaussianBackground
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
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 */
auto
degrees_to_radians_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

/**
 * @name detect_maxima_box
 * @brief Detects local maxima in a given square/cubic neighborhood. Pixels in the resulting image are set to 1 if there
 * is no other pixel in a given radius which has a higher intensity, and to 0 otherwise.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius along the x axis. [float ( = 1 )]
 * @param radius_y Radius along the y axis. [float ( = 1 )]
 * @param radius_z Radius along the z axis. [float ( = 1 )]
 * @return Array::Pointer
 *
 * @note 'binarize', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_detectMaximaBox
 * @deprecated This method is deprecated. Consider using detect_maxima() instead.
 */
auto
detect_maxima_box_func(const Device::Pointer & device,
                       const Array::Pointer &  src,
                       Array::Pointer          dst,
                       float                   radius_x,
                       float                   radius_y,
                       float                   radius_z) -> Array::Pointer;

/**
 * @name detect_maxima
 * @brief Detects local maxima in a given square/cubic neighborhood. Pixels in the resulting image are set to 1 if there
 * is no other pixel in a given radius which has a higher intensity, and to 0 otherwise.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius along the x axis. [float ( = 1 )]
 * @param radius_y Radius along the y axis. [float ( = 1 )]
 * @param radius_z Radius along the z axis. [float ( = 1 )]
 * @param connectivity Element shape, "box" or "sphere" [std::string ( = "box" )]
 * @return Array::Pointer
 *
 * @note 'binarize', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_detectMaximaBox
 * @see https://clij.github.io/clij2-docs/reference_detectMaximaSphere
 */
auto
detect_maxima_func(const Device::Pointer & device,
                   const Array::Pointer &  src,
                   Array::Pointer          dst,
                   float                   radius_x,
                   float                   radius_y,
                   float                   radius_z,
                   std::string             connectivity) -> Array::Pointer;


/**
 * @name detect_minima_box
 * @brief Detects local maxima in a given square/cubic neighborhood. Pixels in the resulting image are set to 1 if there
 * is no other pixel in a given radius which has a lower intensity, and to 0 otherwise.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius along the x axis. [float ( = 1 )]
 * @param radius_y Radius along the y axis. [float ( = 1 )]
 * @param radius_z Radius along the z axis. [float ( = 1 )]
 * @return Array::Pointer
 *
 * @note 'binarize', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_detectMinimaBox
 * @deprecated This method is deprecated. Consider using detect_minima() instead.
 */
auto
detect_minima_box_func(const Device::Pointer & device,
                       const Array::Pointer &  src,
                       Array::Pointer          dst,
                       float                   radius_x,
                       float                   radius_y,
                       float                   radius_z) -> Array::Pointer;

/**
 * @name detect_minima
 * @brief Detects local maxima in a given square/cubic neighborhood. Pixels in the resulting image are set to 1 if there
 * is no other pixel in a given radius which has a lower intensity, and to 0 otherwise.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius along the x axis. [float ( = 1 )]
 * @param radius_y Radius along the y axis. [float ( = 1 )]
 * @param radius_z Radius along the z axis. [float ( = 1 )]
 * @param connectivity Element shape, "box" or "sphere" [std::string ( = "box" )]
 * @return Array::Pointer
 *
 * @note 'binarize', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_detectMinimaBox
 * @see https://clij.github.io/clij2-docs/reference_detectMinimaSphere
 */
auto
detect_minima_func(const Device::Pointer & device,
                   const Array::Pointer &  src,
                   Array::Pointer          dst,
                   float                   radius_x,
                   float                   radius_y,
                   float                   radius_z,
                   std::string             connectivity) -> Array::Pointer;

/**
 * @name difference_of_gaussian
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
 * @note 'filter', 'background removal', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_differenceOfGaussian3D
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
 * @brief Takes a label map image and dilates the regions using a octagon shape until they touch. The resulting label
 * map is written to the output.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_extendLabelingViaVoronoi
 */
auto
extend_labeling_via_voronoi_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name invert
 * @brief Computes the negative value of all pixels in a given image. It is recommended to convert images to 32bit float
 * before applying this operation. <pre>f(x) = x</pre> For binary images, use binaryNot.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'filter'
 * @see https://clij.github.io/clij2-docs/reference_invert
 */
auto
invert_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name label_spots
 * @brief Transforms a binary image with single pixles set to 1 to a labelled spots image. Transforms a spots image as
 * resulting from maximum/minimum detection in an image of the same size where every spot has a number 1, 2,... n.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'label', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_labelSpots
 */
auto
label_spots_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name large_hessian_eigenvalue
 * @brief Determines the Hessian eigenvalues and returns the large eigenvalue image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 * @note 'filter', 'in assistant'
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
 * @param src Input image to process. [const Array::Pointer &]
 * @return float
 *
 * @see https://clij.github.io/clij2-docs/reference_maximumOfAllPixels
 *
 */
auto
maximum_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float;


/**
 * @name minimum_of_all_pixels
 * @brief Determines the minimum of all pixels in a given image. It will be stored in a new row of ImageJs Results table
 * in the column 'Min'.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @return float
 *
 * @see https://clij.github.io/clij2-docs/reference_minimumOfAllPixels
 *
 */
auto
minimum_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float;


/**
 * @name minimum_of_masked_pixels
 * @brief Determines the minimum intensity in a masked image. But only in pixels which have nonzero values in another
 * mask image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param mask Input mask. [const Array::Pointer &]
 * @return float
 *
 * @see https://clij.github.io/clij2-docs/reference_minimumOfMaskedPixels
 *
 */
auto
minimum_of_masked_pixels_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & mask)
  -> float;


/**
 * @name opening_box
 * @brief Opening operator, applies morphological opening to intensity images using a boxshaped footprint.
 * This operator also works with binary images.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius along the x axis. [float ( = 1 )]
 * @param radius_y Radius along the y axis. [float ( = 1 )]
 * @param radius_z Radius along the z axis. [float ( = 1 )]
 * @return Array::Pointer
 * @note 'filter', 'in assistant'
 * @deprecated This method is deprecated. Consider using opening() instead.
 */
auto
opening_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 float                   radius_x,
                 float                   radius_y,
                 float                   radius_z) -> Array::Pointer;


/**
 * @name opening_sphere
 * @brief Opening operator, applies morphological opening to intensity images using a sphereshaped
 * footprint. This operator also works with binary images.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius along the x axis. [float ( = 1 )]
 * @param radius_y Radius along the y axis. [float ( = 1 )]
 * @param radius_z Radius along the z axis. [float ( = 1 )]
 * @return Array::Pointer
 * @note 'filter', 'in assistant'
 * @deprecated This method is deprecated. Consider using opening() instead.
 */
auto
opening_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z) -> Array::Pointer;


/**
 * @name grayscale_opening
 * @brief Opening operator, Applies morphological opening to intensity images using a sphereshaped or
 * boxshepd footprint. This operator also works with binary images.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius along the x axis. [float ( = 1 )]
 * @param radius_y Radius along the y axis. [float ( = 1 )]
 * @param radius_z Radius along the z axis. [float ( = 1 )]
 * @param connectivity Element shape, "box" or "sphere" [std::string ( = "box" )]
 * @return Array::Pointer
 * @note 'filter', 'in assistant'
 *
 */
auto
grayscale_opening_func(const Device::Pointer & device,
                       const Array::Pointer &  src,
                       Array::Pointer          dst,
                       float                   radius_x,
                       float                   radius_y,
                       float                   radius_z,
                       std::string             connectivity) -> Array::Pointer;

/**
 * @name opening
 * @brief Closing operator, applies morphological opening to intensity images using a custom structuring element
 * provided as input. This operator also works with binary images.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param footprint Structuring element for the operation. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 * @note 'filter', 'in assistant'
 */
auto
opening_func(const Device::Pointer & device,
             const Array::Pointer &  src,
             const Array::Pointer &  footprint,
             Array::Pointer          dst) -> Array::Pointer;

/**
 * @name binary_opening
 * @brief Closing operator, applies binary morphological opening to intensity images using a sphere or box shaped
 * footprint. This operator also works with binary images.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius of the sphere or box element along the x axis. [float ( = 1 )]
 * @param radius_y Radius of the sphere or box element along the y axis. [float ( = 1 )]
 * @param radius_z Radius of the sphere or box element along the z axis. [float ( = 1 )]
 * @param connectivity Element shape, "box" or "sphere" [std::string ( = "box" )]
 * @return Array::Pointer
 * @note 'filter', 'in assistant'
 */
auto
binary_opening_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z,
                    std::string             connectivity) -> Array::Pointer;

/**
 * @name radians_to_degrees
 * @brief Converts radians to degrees
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 */
auto
radians_to_degrees_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name reduce_labels_to_label_edges
 * @brief Takes a label map and reduces all labels to their edges. Label IDs stay and background will be zero.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_reduceLabelsToLabelEdges
 */
auto
reduce_labels_to_label_edges_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name small_hessian_eigenvalue
 * @brief Determines the Hessian eigenvalues and returns the small eigenvalue image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 * @note 'filter', 'in assistant'
 *
 */
auto
small_hessian_eigenvalue_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name square
 * @brief Return the elementwise square of the input. This function is supposed to be similar to its counterpart in
 * numpy [1]
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'filter'
 * @see https://numpy.org/doc/stable/reference/generated/numpy.square.html
 */
auto
square_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name squared_difference
 * @brief Determines the squared difference pixel by pixel between two images.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 First input image. [const Array::Pointer &]
 * @param src1 Second input image. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'combine', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_squaredDifference
 */
auto
squared_difference_func(const Device::Pointer & device,
                        const Array::Pointer &  src0,
                        const Array::Pointer &  src1,
                        Array::Pointer          dst) -> Array::Pointer;


/**
 * @name standard_deviation_box
 * @brief Computes the local standard deviation of a pixels box neighborhood. The box size is specified by its
 * halfwidth, halfheight and halfdepth (radius). If 2D images are given, radius_z will be ignored.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius along the x axis. [float ( = 1 )]
 * @param radius_y Radius along the y axis. [float ( = 1 )]
 * @param radius_z Radius along the z axis. [float ( = 1 )]
 * @return Array::Pointer
 *
 * @note 'filter', 'edge detection', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_standardDeviationBox
 * @deprecated This method is deprecated. Consider using standard_deviation() instead.
 */
auto
standard_deviation_box_func(const Device::Pointer & device,
                            const Array::Pointer &  src,
                            Array::Pointer          dst,
                            float                   radius_x,
                            float                   radius_y,
                            float                   radius_z) -> Array::Pointer;


/**
 * @name standard_deviation_sphere
 * @brief Computes the local standard deviation of a pixels sphere neighborhood. The box size is specified by its
 * halfwidth, halfheight and halfdepth (radius). If 2D images are given, radius_z will be ignored.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius along the x axis. [float ( = 1 )]
 * @param radius_y Radius along the y axis. [float ( = 1 )]
 * @param radius_z Radius along the z axis. [float ( = 1 )]
 * @return Array::Pointer
 *
 * @note 'filter', 'edge detection', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_standardDeviationSphere
 * @deprecated This method is deprecated. Consider using standard_deviation() instead.
 */
auto
standard_deviation_sphere_func(const Device::Pointer & device,
                               const Array::Pointer &  src,
                               Array::Pointer          dst,
                               float                   radius_x,
                               float                   radius_y,
                               float                   radius_z) -> Array::Pointer;

/**
 * @name standard_deviation
 * @brief Computes the local standard deviation of a pixels sphere neighborhood. The box size is specified by its
 * halfwidth, halfheight and halfdepth (radius). If 2D images are given, radius_z will be ignored.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius along the x axis. [float ( = 1 )]
 * @param radius_y Radius along the y axis. [float ( = 1 )]
 * @param radius_z Radius along the z axis. [float ( = 1 )]
 * @param connectivity Neigborhood shape, "box" or "sphere" [std::string ( = "box" )]
 * @return Array::Pointer
 *
 * @note 'filter', 'edge detection', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_standardDeviationBox
 * @see https://clij.github.io/clij2-docs/reference_standardDeviationSphere
 */
auto
standard_deviation_func(const Device::Pointer & device,
                        const Array::Pointer &  src,
                        Array::Pointer          dst,
                        float                   radius_x,
                        float                   radius_y,
                        float                   radius_z,
                        std::string             connectivity) -> Array::Pointer;

/**
 * @name subtract_gaussian_background
 * @brief Applies Gaussian blur to the input image and subtracts the result from the original.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param sigma_x Radius along the x axis. [float ( = 2 )]
 * @param sigma_y Radius along the y axis. [float ( = 2 )]
 * @param sigma_z Radius along the z axis. [float ( = 2 )]
 * @return Array::Pointer
 *
 * @note 'filter', 'background removal', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_subtractGaussianBackground
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
 * @brief Subtracts one image X from another image Y pixel wise. <pre>f(x, y) = x y</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 First input image. [const Array::Pointer &]
 * @param src1 Second input image. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'combine', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_subtractImages
 */
auto
subtract_images_func(const Device::Pointer & device,
                     const Array::Pointer &  src0,
                     const Array::Pointer &  src1,
                     Array::Pointer          dst) -> Array::Pointer;

/**
 * @name sub_stack
 * @brief Crop a volume into a new volume, along the z-axis.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image. [const Array::Pointer &]
 * @param dst Output image. [Array::Pointer ( = None )]
 * @param start_z Start z coordinate of the crop. [int ( = 0 )]
 * @param end_z End z coordinate of the crop. [int ( = 0 )]
 * @return Array::Pointer
 *
 * @note 'transform', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_subStack
 */
auto
sub_stack_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int start_z, int end_z)
  -> Array::Pointer;

/**
 * @name reduce_stack
 * @brief Reduces the number of z-slices in a stack by a given factor. With the offset you have control which slices
 * stays: with a factor 3 and offset 0, slices 0,3,6, etc. are kept. with a factor 4 and offset 1, slices 1,5,9, etc.
 * are kept.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image. [const Array::Pointer &]
 * @param dst Output image. [Array::Pointer ( = None )]
 * @param reduction_factor Reduction factor. [int ( = 2 )]
 * @param offset Offset. [int ( = 0 )]
 * @return Array::Pointer
 *
 * @note 'transform', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_reduceStack
 */
auto
reduce_stack_func(const Device::Pointer & device,
                  const Array::Pointer &  src,
                  Array::Pointer          dst,
                  int                     reduction_factor,
                  int                     offset) -> Array::Pointer;

/**
 * @name sum_of_all_pixels
 * @brief Determines the sum of all pixels in a given image. It will be stored in a new row of ImageJs Results table in
 * the column 'Sum'.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer & ( = None )]
 * @return float
 * @see https://clij.github.io/clij2-docs/reference_sumOfAllPixels
 *
 */
auto
sum_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float;


/**
 * @name top_hat_box
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
 * @note 'filter', 'background removal', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_topHatBox
 * @deprecated This method is deprecated. Consider using top_hat() instead.
 */
auto
top_hat_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 float                   radius_x,
                 float                   radius_y,
                 float                   radius_z) -> Array::Pointer;


/**
 * @name top_hat_sphere
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
 * @note 'filter', 'background removal', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_topHatSphere
 * @deprecated This method is deprecated. Consider using top_hat() instead.
 */
auto
top_hat_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z) -> Array::Pointer;

/**
 * @name top_hat
 * @brief Applies a tophat filter for background subtraction to the input image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src The input image where the background is subtracted from. [const Array::Pointer &]
 * @param dst The output image where results are written into. [Array::Pointer ( = None )]
 * @param radius_x Radius of the background determination region in X. [float ( = 1 )]
 * @param radius_y Radius of the background determination region in Y. [float ( = 1 )]
 * @param radius_z Radius of the background determination region in Z. [float ( = 1 )]
 * @param connectivity Element shape, "box" or "sphere" [std::string ( = "box" )]
 * @return Array::Pointer
 *
 * @note 'filter', 'background removal', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_topHatBox
 * @see https://clij.github.io/clij2-docs/reference_topHatSphere
 */
auto
top_hat_func(const Device::Pointer & device,
             const Array::Pointer &  src,
             Array::Pointer          dst,
             float                   radius_x,
             float                   radius_y,
             float                   radius_z,
             std::string             connectivity) -> Array::Pointer;

} // namespace cle::tier2

#endif // __INCLUDE_TIER2_HPP
