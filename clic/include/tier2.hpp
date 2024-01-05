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
 * @brief Determines the absolute difference pixel by pixel between two images.  <pre>f(x, y) = |x - y| </pre>
 * @category 'combine', 'in assistant', 'bia-bob-suggestion'
 * @link https://clij.github.io/clij2-docs/reference_absoluteDifference
 */
auto
absolute_difference_func(const Device::Pointer & device,
                         const Array::Pointer &  src0,
                         const Array::Pointer &  src1,
                         Array::Pointer          dst) -> Array::Pointer;


/**
 * @name add_images
 * @brief Calculates the sum of pairs of pixels x and y of two images X and Y.  <pre>f(x, y) = x + y</pre>
 * @category 'combine', 'in assistant'
 * @link https://clij.github.io/clij2-docs/reference_addImages
 */
auto
add_images_func(const Device::Pointer & device,
                const Array::Pointer &  src0,
                const Array::Pointer &  src1,
                Array::Pointer          dst) -> Array::Pointer;


/**
 * @name bottom_hat_box
 * @brief Apply a bottom-hat filter for background subtraction to the input image.
 * @category 'filter', 'background removal', 'in assistant'
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
 * @brief Applies a bottom-hat filter for background subtraction to the input image.
 * @category 'filter', 'background removal', 'in assistant', 'bia-bob-suggestion'
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
 * @brief Limits the range of values in an image.  This function is supposed to work similarly as its counter part in numpy [1].
 * @category 'combine', 'in assistant'
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
 * @brief Closing operator, box-shaped  Applies morphological closing to intensity images using a box-shaped footprint. This operator also works with binary images.
 * @category 'filter', 'in assistant'

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
 * @brief Closing operator, sphere-shaped  Applies morphological closing to intensity images using a sphere-shaped footprint. This operator also works with binary images.
 * @category 'filter', 'in assistant', 'bia-bob-suggestion'

 */
auto
closing_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z) -> Array::Pointer;


auto
concatenate_along_x_func(const Device::Pointer & device,
                         const Array::Pointer &  src0,
                         const Array::Pointer &  src1,
                         Array::Pointer          dst) -> Array::Pointer;

auto
concatenate_along_y_func(const Device::Pointer & device,
                         const Array::Pointer &  src0,
                         const Array::Pointer &  src1,
                         Array::Pointer          dst) -> Array::Pointer;

auto
concatenate_along_z_func(const Device::Pointer & device,
                         const Array::Pointer &  src0,
                         const Array::Pointer &  src1,
                         Array::Pointer          dst) -> Array::Pointer;

/**
 * @name count_touching_neighbors
 * @brief Takes a touch matrix as input and delivers a vector with number of touching neighbors per label as a vector.  Note: Background is considered as something that can touch. To ignore touches with background, hand over a touch matrix where the first column (index = 0) has been set to 0. Use set_column for that.
 * @category 'bia-bob-suggestion'
 * @link https://clij.github.io/clij2-docs/reference_countTouchingNeighbors
 */
auto
count_touching_neighbors_func(const Device::Pointer & device,
                              const Array::Pointer &  src,
                              Array::Pointer          dst,
                              bool                    ignore_background) -> Array::Pointer;


/**
 * @name crop_border
 * @brief Crops an image by removing the outer pixels, per default 1.  Notes ----- * To make sure the output image has the right size, provide destination_image=None.

 */
auto
crop_border_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int border_size)
  -> Array::Pointer;


/**
 * @name divide_by_gaussian_background
 * @brief Applies Gaussian blur to the input image and divides the original by the result.
 * @category 'filter', 'background removal', 'in assistant', 'bia-bob-suggestion'
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
 * @brief Converts radians to degrees

 */
auto
degrees_to_radians_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name difference_of_gaussian
 * @brief Applies Gaussian blur to the input image twice with different sigma values resulting in two images which are then subtracted from each other.  It is recommended to apply this operation to images of type Float (32 bit) as results might be negative.
 * @category 'filter', 'background removal', 'bia-bob-suggestion'
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
 * @brief Takes a label map image and dilates the regions using a octagon shape until they touch.  The resulting label map is written to the output.
 * @category 'label processing', 'in assistant', 'bia-bob-suggestion'
 * @link https://clij.github.io/clij2-docs/reference_extendLabelingViaVoronoi
 */
auto
extend_labeling_via_voronoi_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name invert
 * @brief Computes the negative value of all pixels in a given image.  It is recommended to convert images to 32-bit float before applying this operation.  <pre>f(x) = - x</pre>  For binary images, use binaryNot.
 * @category 'filter'
 * @link https://clij.github.io/clij2-docs/reference_invert
 */
auto
invert_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name label_spots
 * @brief Transforms a binary image with single pixles set to 1 to a labelled spots image.  Transforms a spots image as resulting from maximum/minimum detection in an image of the same size where every spot has a number 1, 2, ... n.
 * @category 'label', 'in assistant', 'bia-bob-suggestion'
 * @link https://clij.github.io/clij2-docs/reference_labelSpots
 */
auto
label_spots_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name large_hessian_eigenvalue
 * @brief Determines the Hessian eigenvalues and returns the large eigenvalue image.
 * @category 'filter', 'in assistant'

 */
auto
large_hessian_eigenvalue_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name maximum_of_all_pixels
 * @brief Determines the maximum of all pixels in a given image.  It will be stored in a new row of ImageJs Results table in the column 'Max'.
 * @link https://clij.github.io/clij2-docs/reference_maximumOfAllPixels
 */
auto
maximum_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float;


/**
 * @name minimum_of_all_pixels
 * @brief Determines the minimum of all pixels in a given image.  It will be stored in a new row of ImageJs Results table in the column 'Min'.
 * @link https://clij.github.io/clij2-docs/reference_minimumOfAllPixels
 */
auto
minimum_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float;


/**
 * @name minimum_of_masked_pixels
 * @brief Determines the minimum intensity in a masked image.  But only in pixels which have non-zero values in another mask image.
 * @link https://clij.github.io/clij2-docs/reference_minimumOfMaskedPixels
 */
auto
minimum_of_masked_pixels_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & mask)
  -> float;


/**
 * @name opening_box
 * @brief Opening operator, box-shaped  Applies morphological opening to intensity images using a box-shaped footprint. This operator also works with binary images.
 * @category 'filter', 'in assistant'

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
 * @brief Opening operator, sphere-shaped  Applies morphological opening to intensity images using a sphere-shaped footprint. This operator also works with binary images.
 * @category 'filter', 'in assistant', 'bia-bob-suggestion'

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

 */
auto
radians_to_degrees_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name small_hessian_eigenvalue
 * @brief Determines the Hessian eigenvalues and returns the small eigenvalue image.
 * @category 'filter', 'in assistant'

 */
auto
small_hessian_eigenvalue_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name square
 * @brief Return the element-wise square of the input.  This function is supposed to be similar to its counterpart in numpy [1]
 * @category 'filter'
 * @link https://numpy.org/doc/stable/reference/generated/numpy.square.html
 */
auto
square_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name squared_difference
 * @brief Determines the squared difference pixel by pixel between two images.
 * @category 'combine', 'in assistant', 'bia-bob-suggestion'
 * @link https://clij.github.io/clij2-docs/reference_squaredDifference
 */
auto
squared_difference_func(const Device::Pointer & device,
                        const Array::Pointer &  src0,
                        const Array::Pointer &  src1,
                        Array::Pointer          dst) -> Array::Pointer;


/**
 * @name standard_deviation_box
 * @brief Computes the local standard deviation of a pixels box neighborhood. The box size is specified by its half-width, half-height and half-depth (radius). If 2D images are given, radius_z will be ignored.
 * @category 'filter', 'edge detection', 'in assistant'
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
 * @brief Computes the local standard deviation of a pixels sphere neighborhood. The box size is specified by its half-width, half-height and half-depth (radius). If 2D images are given, radius_z will be ignored.
 * @category 'filter', 'edge detection', 'in assistant', 'bia-bob-suggestion'
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
 * @brief Applies Gaussian blur to the input image and subtracts the result from the original.
 * @category 'filter', 'background removal', 'in assistant', 'bia-bob-suggestion'
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
 * @brief Subtracts one image X from another image Y pixel wise.  <pre>f(x, y) = x - y</pre>
 * @category 'combine', 'in assistant'
 * @link https://clij.github.io/clij2-docs/reference_subtractImages
 */
auto
subtract_images_func(const Device::Pointer & device,
                     const Array::Pointer &  src0,
                     const Array::Pointer &  src1,
                     Array::Pointer          dst) -> Array::Pointer;


/**
 * @name sum_of_all_pixels
 * @brief Determines the sum of all pixels in a given image.  It will be stored in a new row of ImageJs Results table in the column 'Sum'.
 * @link https://clij.github.io/clij2-docs/reference_sumOfAllPixels
 */
auto
sum_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float;


/**
 * @name top_hat_box
 * @brief Applies a top-hat filter for background subtraction to the input image.
 * @category 'filter', 'background removal', 'in assistant'
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
 * @brief Applies a top-hat filter for background subtraction to the input image.
 * @category 'filter', 'background removal', 'in assistant', 'bia-bob-suggestion'
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
