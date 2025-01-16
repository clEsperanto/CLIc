#ifndef __INCLUDE_TIER1_HPP
#define __INCLUDE_TIER1_HPP

#include "tier0.hpp"

/**
 * @namespace cle::tier1
 * @brief Namespace container for all functions of tier 1 category
 *        Tier 1 functions are advanced functions that may rely on previous tier functions
 */
namespace cle::tier1
{
/**
 * @name absolute
 * @brief Computes the absolute value of every individual pixel x in a given image. <pre>f(x) = |x| </pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src The input image to be processed. [const Array::Pointer &]
 * @param dst Output image where results are written into. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'filter', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_absolute
 */
auto
absolute_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name add_images_weighted
 * @brief Calculates the sum of pairs of pixels x and y from images X and Y weighted with factors a and b. <pre>f(x, y,
 * a, b) = x * a + y * b</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 First input image to add. [const Array::Pointer &]
 * @param src1 Second image to add. [const Array::Pointer &]
 * @param dst Output image where results are written into. [Array::Pointer ( = None )]
 * @param factor1 Multiplication factor of each pixel of src0 before adding it. [float ( = 1 )]
 * @param factor2 Multiplication factor of each pixel of src1 before adding it. [float ( = 1 )]
 * @return Array::Pointer
 *
 * @note 'combine', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_addImagesWeighted
 */
auto
add_images_weighted_func(const Device::Pointer & device,
                         const Array::Pointer &  src0,
                         const Array::Pointer &  src1,
                         Array::Pointer          dst,
                         float                   factor1,
                         float                   factor2) -> Array::Pointer;


/**
 * @name add_image_and_scalar
 * @brief Adds a scalar value s to all pixels x of a given image X. <pre>f(x, s) = x + s</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output image. [Array::Pointer ( = None )]
 * @param scalar Scalar number to add to all pixels. [float ( = 1 )]
 * @return Array::Pointer
 *
 * @note 'filter', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_addImageAndScalar
 */
auto
add_image_and_scalar_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar)
  -> Array::Pointer;


/**
 * @name binary_and
 * @brief Computes a binary image (containing pixel values 0 and 1) from two images X and Y by connecting pairs of
 * pixels x and y with the binary AND operator &. All pixel values except 0 in the input images are interpreted as 1.
 * <pre>f(x, y) = x & y</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 First binary input image to be processed. [const Array::Pointer &]
 * @param src1 Second binary input image to be processed. [const Array::Pointer &]
 * @param dst Output image where results are written into. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'combine', 'binary processing', 'in assistant', 'combine labels', 'label processing'
 * @see https://clij.github.io/clij2-docs/reference_binaryAnd
 */
auto
binary_and_func(const Device::Pointer & device,
                const Array::Pointer &  src0,
                const Array::Pointer &  src1,
                Array::Pointer          dst) -> Array::Pointer;


/**
 * @name binary_edge_detection
 * @brief Determines pixels/voxels which are on the surface of binary objects and sets only them to 1 in the destination
 * image. All other pixels are set to 0.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Binary input image where edges will be searched. [const Array::Pointer &]
 * @param dst Output image where edge pixels will be 1. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'binary processing', 'label processing', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_binaryEdgeDetection
 */
auto
binary_edge_detection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name binary_not
 * @brief Computes a binary image (containing pixel values 0 and 1) from an image X by negating its pixel values x using
 * the binary NOT operator ! All pixel values except 0 in the input image are interpreted as 1. <pre>f(x) = !x</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Binary input image to be inverted. [const Array::Pointer &]
 * @param dst Output image where results are written into. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'binary processing', 'filter', 'label processing', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_binaryNot
 */
auto
binary_not_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name binary_or
 * @brief Computes a binary image (containing pixel values 0 and 1) from two images X and Y by connecting pairs of
 * pixels x and y with the binary OR operator |. All pixel values except 0 in the input images are interpreted
 * as 1.<pre>f(x, y) = x | y</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 First binary input image to be processed. [const Array::Pointer &]
 * @param src1 Second binary input image to be processed. [const Array::Pointer &]
 * @param dst Output image where results are written into. [Array::Pointer ( = None )]
 * @return Array::Pointer
 * @note 'combine', 'binary processing', 'in assistant', 'combine labels', 'label processing'
 *
 * @see https://clij.github.io/clij2-docs/reference_binaryOr
 */
auto
binary_or_func(const Device::Pointer & device,
               const Array::Pointer &  src0,
               const Array::Pointer &  src1,
               Array::Pointer          dst) -> Array::Pointer;


/**
 * @name binary_subtract
 * @brief Subtracts one binary image from another.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 First binary input image to be processed. [const Array::Pointer &]
 * @param src1 Second binary input image to be subtracted from the first. [const Array::Pointer &]
 * @param dst Output image where results are written into. [Array::Pointer ( = None )]
 * @return Array::Pointer
 * @note 'combine', 'binary processing', 'in assistant', 'combine labels', 'label processing'
 *
 * @see https://clij.github.io/clij2-docs/reference_binarySubtract
 */
auto
binary_subtract_func(const Device::Pointer & device,
                     const Array::Pointer &  src0,
                     const Array::Pointer &  src1,
                     Array::Pointer          dst) -> Array::Pointer;


/**
 * @name binary_xor
 * @brief Computes a binary image (containing pixel values 0 and 1) from two images X and Y by connecting pairs of
 * pixels x and y with the binary operators AND &, OR | and NOT ! implementing the XOR operator. All pixel values except
 * 0 in the input images are interpreted as 1. <pre>f(x, y) = (x & !y) | (!x & y)</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 First binary input image to be processed. [const Array::Pointer &]
 * @param src1 Second binary input image to be processed. [const Array::Pointer &]
 * @param dst Output image where results are written into. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'combine', 'binary processing', 'in assistant', 'combine labels', 'label processing'
 * @see https://clij.github.io/clij2-docs/reference_binaryXOr
 */
auto
binary_xor_func(const Device::Pointer & device,
                const Array::Pointer &  src0,
                const Array::Pointer &  src1,
                Array::Pointer          dst) -> Array::Pointer;

/**
 * @name binary_supinf
 * @brief Compute the maximum of the erosion with plannar structuring elements.
 * Warning: This operation is only supported BINARY data type images.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src The binary input image to be processed. [const Array::Pointer &]
 * @param dst Output image where results are written into. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'filter', 'binary processing'
 */
auto
binary_supinf_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name binary_infsup
 * @brief Compute the minimum of the dilation with plannar structuring elements.
 * Warning: This operation is only supported BINARY data type images.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src The binary input image to be processed. [const Array::Pointer &]
 * @param dst Output image where results are written into. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'filter', 'binary processing'
 */
auto
binary_infsup_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

/**
 * @name block_enumerate
 * @brief Enumerates pixels with value 1 in a onedimensional image For example handing over the image [0, 1, 1, 0, 1, 0,
 * 1, 1] would be processed to an image [0, 1, 2, 0, 3, 0, 4, 5] This functionality is important in connected component
 * neccessary (see also sum_reduction_x). In the above example, with blocksize 4, that would be the sum array: [2, 3]
 * labeling. Processing is accelerated by paralellization in blocks. Therefore, handing over precomputed block sums is
 * Note that the block size when calling this function and sum_reduction must be identical
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src input binary vector image [const Array::Pointer &]
 * @param sums precomputed sums of blocks [const Array::Pointer &]
 * @param dst output enumerated vector image [Array::Pointer ( = None )]
 * @param blocksize blocksize; must correspond correctly to how the [int ( = 256 )]
 * @return Array::Pointer
 *
 */
auto
block_enumerate_func(const Device::Pointer & device,
                     const Array::Pointer &  src,
                     const Array::Pointer &  sums,
                     Array::Pointer          dst,
                     int                     blocksize) -> Array::Pointer;


/**
 * @name convolve
 * @brief Convolve the image with a given kernel image. It is recommended that the kernel image has an odd size in X, Y
 * and Z.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to convolve. [const Array::Pointer &]
 * @param kernel Kernel image to use for the convolution. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'filter', 'combine', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_convolve
 */
auto
convolve_func(const Device::Pointer & device,
              const Array::Pointer &  src,
              const Array::Pointer &  kernel,
              Array::Pointer          dst) -> Array::Pointer;


/**
 * @name copy
 * @brief Copies an image. <pre>f(x) = x</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to copy. [const Array::Pointer &]
 * @param dst Output copy image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_copy
 *
 */
auto
copy_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name copy_slice
 * @brief This method has two purposes: It copies a 2D image to a given slice_index z position in a 3D image stack or It
 * copies a given slice_index at position z in an image stack to a 2D image. The first case is only available via ImageJ
 * macro. If you are using it, it is recommended that the target 3D image already preexists in GPU memory before calling
 * this method. Otherwise, CLIJ create the image stack with z planes.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to copy from. [const Array::Pointer &]
 * @param dst Output copy image slice_index. [Array::Pointer ( = None )]
 * @param slice_index Index of the slice to copy. [int ( = 0 )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_copySlice
 *
 */
auto
copy_slice_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int slice_index)
  -> Array::Pointer;


/**
 * @name copy_horizontal_slice
 * @brief This method has two purposes: It copies a 2D image to a given slice_index y position in a 3D image stack or It
 * copies a given slice_index at position y in an image stack to a 2D image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to copy from. [const Array::Pointer &]
 * @param dst Output copy image slice_index. [Array::Pointer ( = None )]
 * @param slice_index Index of the slice to copy. [int ( = 0 )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_copySlice
 *
 */
auto
copy_horizontal_slice_func(const Device::Pointer & device,
                           const Array::Pointer &  src,
                           Array::Pointer          dst,
                           int                     slice_index) -> Array::Pointer;


/**
 * @name copy_vertical_slice
 * @brief This method has two purposes: It copies a 2D image to a given slice_index x position in a 3D image stack or It
 * copies a given slice_index at position x in an image stack to a 2D image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to copy from. [const Array::Pointer &]
 * @param dst Output copy image slice_index. [Array::Pointer ( = None )]
 * @param slice_index Index of the slice to copy. [int ( = 0 )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_copySlice
 *
 */
auto
copy_vertical_slice_func(const Device::Pointer & device,
                         const Array::Pointer &  src,
                         Array::Pointer          dst,
                         int                     slice_index) -> Array::Pointer;


/**
 * @name crop
 * @brief Crops a given substack out of a given image stack. Note: If the destination image preexists already, it will
 * be overwritten and keep it's dimensions.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param start_x Starting index coordicante x. [int ( = 0 )]
 * @param start_y Starting index coordicante y. [int ( = 0 )]
 * @param start_z Starting index coordicante z. [int ( = 0 )]
 * @param width Width size of the region to crop. [int ( = 1 )]
 * @param height Height size of the region to crop. [int ( = 1 )]
 * @param depth Depth size of the region to crop. [int ( = 1 )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_crop3D
 *
 */
auto
crop_func(const Device::Pointer & device,
          const Array::Pointer &  src,
          Array::Pointer          dst,
          int                     start_x,
          int                     start_y,
          int                     start_z,
          int                     width,
          int                     height,
          int                     depth) -> Array::Pointer;


/**
 * @name cubic_root
 * @brief Computes the cubic root of each pixel.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'filter', 'in assistant'
 */
auto
cubic_root_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name detect_label_edges
 * @brief Takes a labelmap and returns an image where all pixels on label edges are set to 1 and all other pixels to 0.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'binarize', 'label processing', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_detectLabelEdges
 */
auto
detect_label_edges_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;

/**
 * @name dilation
 * @brief Computes the dilation operation between an image and a structuring element. The operation is applied in
 * grayscale if the image is in grayscale. The structuring element is a binary image with pixel values 0 and 1, and must
 * have the same dimensionality as the image (3D is the image is 3D, 2D if the image is 2D).
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param footprint Structuring element to use for the operation. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'binary processing' 'filter'
 * @see https://clij.github.io/clij2-docs/reference_erodeBox
 * @deprecated This function is deprecated. Consider using erode() instead.
 */
auto
dilation_func(const Device::Pointer & device,
              const Array::Pointer &  src,
              const Array::Pointer &  footprint,
              Array::Pointer          dst) -> Array::Pointer;

/**
 * @name dilate_box
 * @brief Computes a binary image with pixel values 0 and 1 containing the binary dilation of a given input image. The
 * dilation takes the Moore neighborhood (8 pixels in 2D and 26 pixels in 3d) into account. The pixels in the input
 * image with pixel value not equal to 0 will be interpreted as 1. This method is comparable to the 'Dilate' menu in
 * ImageJ in case it is applied to a 2D image. The only difference is that the output image contains values 0 and 1
 * instead of 0 and 255.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. Input image to process. [const Array::Pointer &]
 * @param dst Output result image. Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'binary processing'
 * @see https://clij.github.io/clij2-docs/reference_dilateBox
 * @deprecated This function is deprecated. Consider using dilate() instead.
 */
auto
dilate_box_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name dilate_sphere
 * @brief Computes a binary image with pixel values 0 and 1 containing the binary dilation of a given input image. The
 * dilation takes the von Neumann neighborhood (4 pixels in 2D and 6 pixels in 3d) into account. The pixels in the input
 * image with pixel value not equal to 0 will be interpreted as 1.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. Input image to process. [const Array::Pointer &]
 * @param dst Output result image. Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'binary processing'
 * @see https://clij.github.io/clij2-docs/reference_dilateSphere
 * @deprecated This function is deprecated. Consider using dilate() instead.
 */
auto
dilate_sphere_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name binary_dilate
 * @brief Computes a binary image with pixel values 0 and 1 containing the binary dilation of a given input image. The
 * dilation apply the Moore neighborhood (8 pixels in 2D and 26 pixels in 3d) for the "box" connectivity and
 * the von Neumann neighborhood (4 pixels in 2D and 6 pixels in 3d) for a "sphere" connectivity.
 * The pixels in the input image with pixel value not equal to 0 will be interpreted as 1.
 *
 * For a more flexible dilation with arbitrary shapes, use dilation() instead.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. Input image to process. [const Array::Pointer &]
 * @param dst Output result image. Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius of sphere or box structuring element in X. [float ( = 1 )]
 * @param radius_y Radius of sphere or box structuring element in Y. [float ( = 1 )]
 * @param radius_z Radius of sphere or box structuring element in Z. [float ( = 1 )]
 * @param connectivity Element shape, "box" or "sphere". [std::string ( = "box" )]
 * @return Array::Pointer
 *
 * @note 'binary processing'
 * @see https://clij.github.io/clij2-docs/reference_dilateBox
 * @see https://clij.github.io/clij2-docs/reference_dilateSphere
 */
auto
binary_dilate_func(const Device::Pointer & device,
                   const Array::Pointer &  src,
                   Array::Pointer          dst,
                   float                   radius_x,
                   float                   radius_y,
                   float                   radius_z,
                   std::string             connectivity) -> Array::Pointer;


/**
 * @name divide_images
 * @brief Divides two images X and Y by each other pixel wise. <pre>f(x, y) = x / y</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param dividend Input image to process. [const Array::Pointer &]
 * @param divisor Second input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'combine', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_divideImages
 */
auto
divide_images_func(const Device::Pointer & device,
                   const Array::Pointer &  dividend,
                   const Array::Pointer &  divisor,
                   Array::Pointer          dst) -> Array::Pointer;


/**
 * @name divide_scalar_by_image
 * @brief Divides a scalar by an image pixel by pixel. <pre>f(x, s) = s / x</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param scalar Scalar value to divide the image with. [float ( = 0 )]
 * @return Array::Pointer
 *
 * @note 'filter', 'in assistant'
 */
auto
divide_scalar_by_image_func(const Device::Pointer & device,
                            const Array::Pointer &  src,
                            Array::Pointer          dst,
                            float                   scalar) -> Array::Pointer;

/**
 * @name equal
 * @brief Determines if two images A and B equal pixel wise. <pre>f(a, b) = 1 if a == b; 0 otherwise.</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 First image to be compared with. [const Array::Pointer &]
 * @param src1 Second image to be compared with the first. [const Array::Pointer &]
 * @param dst Output binary image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'combine', 'binarize', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_equal
 */
auto
equal_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name equal_constant
 * @brief Determines if an image A and a constant b are equal. <pre>f(a, b) = 1 if a == b; 0 otherwise.</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input omage where every pixel is compared to the constant. [const Array::Pointer &]
 * @param dst Output binary image. [Array::Pointer ( = None )]
 * @param constant Scalar value to compare pixel with. [float ( = 0 )]
 * @return Array::Pointer
 *
 * @note 'binarize', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_equalConstant
 */
auto
equal_constant_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float constant)
  -> Array::Pointer;

/**
 * @name erosion
 * @brief Computes the erosion operation between an image and a structuring element. The operation is applied in
 * grayscale if the image is in grayscale. The structuring element is a binary image with pixel values 0 and 1, and must
 * have the same dimensionality as the image (3D is the image is 3D, 2D if the image is 2D).
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param footprint Structuring element to use for the operation. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'binary processing' 'filter'
 * @see https://clij.github.io/clij2-docs/reference_erodeBox
 */
auto
erosion_func(const Device::Pointer & device,
             const Array::Pointer &  src,
             const Array::Pointer &  footprint,
             Array::Pointer          dst) -> Array::Pointer;

/**
 * @name erode_box
 * @brief Computes a binary image with pixel values 0 and 1 containing the binary erosion of a given input image. The
 * erosion takes the Moore neighborhood (8 pixels in 2D and 26 pixels in 3d) into account. The pixels in the input image
 * with pixel value not equal to 0 will be interpreted as 1. This method is comparable to the 'Erode' menu in ImageJ in
 * case it is applied to a 2D image. The only difference is that the output image contains values 0 and 1 instead of 0
 * and 255.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'binary processing'
 * @see https://clij.github.io/clij2-docs/reference_erodeBox
 * @deprecated This function is deprecated. Consider using erode() instead.
 */
auto
erode_box_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name erode_sphere
 * @brief Computes a binary image with pixel values 0 and 1 containing the binary erosion of a given input image. The
 * erosion takes the von Neumann neighborhood (4 pixels in 2D and 6 pixels in 3d) into account. The pixels in the input
 * image with pixel value not equal to 0 will be interpreted as 1.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'binary processing'
 * @see https://clij.github.io/clij2-docs/reference_erodeSphere
 * @deprecated This function is deprecated. Consider using erode() instead.
 */
auto
erode_sphere_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name binary_erode
 * @brief Computes a binary image with pixel values 0 and 1 containing the binary erosion of a given input image. The
 * erosion apply the Moore neighborhood (8 pixels in 2D and 26 pixels in 3d) for the "box" connectivity and
 * the von Neumann neighborhood (4 pixels in 2D and 6 pixels in 3d) for a "sphere" connectivity. The pixels in the input
 * image with pixel value not equal to 0 will be interpreted as 1.
 *
 * For a more flexible erosion with arbitrary shapes, use erosion() instead.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius of the eroding sphere or box structuring element in X. [float ( = 1 )]
 * @param radius_y Radius of the eroding sphere or box structuring element in Y. [float ( = 1 )]
 * @param radius_z Radius of the eroding sphere or box structuring element in Z. [float ( = 1 )]
 * @param connectivity Element shape, "box" or "sphere". [std::string ( = "box" )]
 * @return Array::Pointer
 *
 * @note 'binary processing'
 * @see https://clij.github.io/clij2-docs/reference_erodeBox
 * @see https://clij.github.io/clij2-docs/reference_erodeSphere
 */
auto
binary_erode_func(const Device::Pointer & device,
                  const Array::Pointer &  src,
                  Array::Pointer          dst,
                  float                   radius_x,
                  float                   radius_y,
                  float                   radius_z,
                  std::string             connectivity) -> Array::Pointer;


/**
 * @name exponential
 * @brief Computes base exponential of all pixels values. f(x) = exp(x) Author(s): Peter Haub, Robert Haase
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'filter', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_exponential
 */
auto
exponential_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name flip
 * @brief Flips an image in X, Y and/or Z direction depending on boolean flags.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param flip_x Flip along the x axis if true. [bool ( = True )]
 * @param flip_y Flip along the y axis if true. [bool ( = True )]
 * @param flip_z Flip along the z axis if true. [bool ( = True )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_flip3D
 *
 */
auto
flip_func(const Device::Pointer & device,
          const Array::Pointer &  src,
          Array::Pointer          dst,
          bool                    flip_x,
          bool                    flip_y,
          bool                    flip_z) -> Array::Pointer;


/**
 * @name gaussian_blur
 * @brief Computes the Gaussian blurred image of an image given sigma values in X, Y and Z. Thus, the filter kernel can
 * have nonisotropic shape. The implementation is done separable. In case a sigma equals zero, the direction is not
 * blurred.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param sigma_x Sigma value along the x axis. [float ( = 0 )]
 * @param sigma_y Sigma value along the y axis. [float ( = 0 )]
 * @param sigma_z Sigma value along the z axis. [float ( = 0 )]
 * @return Array::Pointer
 *
 * @note 'filter', 'denoise', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_gaussianBlur3D
 */
auto
gaussian_blur_func(const Device::Pointer & device,
                   const Array::Pointer &  src,
                   Array::Pointer          dst,
                   float                   sigma_x,
                   float                   sigma_y,
                   float                   sigma_z) -> Array::Pointer;


/**
 * @name generate_distance_matrix
 * @brief Computes the distance between all point coordinates given in two point lists. Takes two images containing
 * pointlists (dimensionality n * d, n: number of points and d: dimensionality) and builds up a matrix containing the
 * distances between these points. Convention: Given two point lists with dimensionality n * d and m * d, the distance
 * matrix will be of size(n + 1) * (m + 1). The first row and column contain zeros. They represent the distance of the
 * (see generateTouchMatrix). Thus, one can threshold a distance matrix to generate a touch matrix out of it for drawing
 * objects to a theoretical background object. In that way, distance matrices are of the same size as touch matrices
 * meshes.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param coordinate_list1 First coordinate list to process. [const Array::Pointer &]
 * @param coordinate_list2 Second coordinate list to process. [const Array::Pointer &]
 * @param distance_matrix_destination Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @see https://clij.github.io/clij2-docs/reference_generateDistanceMatrix
 */
auto
generate_distance_matrix_func(const Device::Pointer & device,
                              const Array::Pointer &  coordinate_list1,
                              const Array::Pointer &  coordinate_list2,
                              Array::Pointer          distance_matrix_destination) -> Array::Pointer;


/**
 * @name gradient_x
 * @brief Computes the gradient of gray values along X. Assuming a, b and c are three adjacent pixels in X direction. In
 * the target image will be saved as:
 * <pre>b' = c a;</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'filter', 'edge detection', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_gradientX
 */
auto
gradient_x_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name gradient_y
 * @brief Computes the gradient of gray values along Y. Assuming a, b and c are three adjacent pixels in Y direction. In
 * the target image will be saved as:
 * <pre>b' = c a;</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'filter', 'edge detection', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_gradientY
 */
auto
gradient_y_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name gradient_z
 * @brief Computes the gradient of gray values along Z. Assuming a, b and c are three adjacent pixels in Z direction. In
 * the target image will be saved as:
 * <pre>b' = c a;</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'filter', 'edge detection', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_gradientZ
 */
auto
gradient_z_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name greater
 * @brief Determines if two images A and B greater pixel wise. f(a, b) = 1 if a > b; 0 otherwise.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 First input image to process. [const Array::Pointer &]
 * @param src1 Second input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'combine', 'binarize', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_greater
 */
auto
greater_func(const Device::Pointer & device,
             const Array::Pointer &  src0,
             const Array::Pointer &  src1,
             Array::Pointer          dst) -> Array::Pointer;


/**
 * @name greater_constant
 * @brief Determines if two images A and B greater pixel wise. f(a, b) = 1 if a > b; 0 otherwise.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param constant Scalar value to compare pixel with. [float ( = 0 )]
 * @return Array::Pointer
 *
 * @note 'binarize', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_greaterConstant
 */
auto
greater_constant_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float constant)
  -> Array::Pointer;


/**
 * @name greater_or_equal
 * @brief Determines if two images A and B greater or equal pixel wise. f(a, b) = 1 if a >= b; 0 otherwise.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 First input image to process. [const Array::Pointer &]
 * @param src1 Second input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'combine', 'binarize', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_greaterOrEqual
 */
auto
greater_or_equal_func(const Device::Pointer & device,
                      const Array::Pointer &  src0,
                      const Array::Pointer &  src1,
                      Array::Pointer          dst) -> Array::Pointer;


/**
 * @name greater_or_equal_constant
 * @brief Determines if two images A and B greater or equal pixel wise. f(a, b) = 1 if a >= b; 0 otherwise.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param constant Scalar value to compare pixel with. [float ( = 0 )]
 * @return Array::Pointer
 *
 * @note 'binarize', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_greaterOrEqualConstant
 */
auto
greater_or_equal_constant_func(const Device::Pointer & device,
                               const Array::Pointer &  src,
                               Array::Pointer          dst,
                               float                   constant) -> Array::Pointer;


/**
 * @name hessian_eigenvalues
 * @brief Computes the eigenvalues of the hessian matrix of a 2d or 3d image. Hessian matrix or 2D images: [Ixx, Ixy]
 * [Ixy, Iyy] Hessian matrix for 3D images: [Ixx, Ixy, Ixz] [Ixy, Iyy, Iyz] [Ixz, Iyz, Izz] Ixx denotes the second
 * derivative in x. Ixx and Iyy are calculated by convolving the image with the 1d kernel [1 2 1]. Ixy is calculated by
 * a convolution with the 2d kernel: [ 0.25 0 0.25] [ 0 0 0] [0.25 0 0.25] Note: This is the only clesperanto function
 * that returns multiple images. This API might be subject to change in the future. Consider using
 * small_hessian_eigenvalue() and/or large_hessian_eigenvalue() instead which return only one image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param small_eigenvalue Output result image. [Array::Pointer ( = None )]
 * @param middle_eigenvalue Output result image, null if input is 2D. [Array::Pointer ( = None )]
 * @param large_eigenvalue Output result image. [Array::Pointer ( = None )]
 * @return std::vector<Array::Pointer>
 *
 */
auto
hessian_eigenvalues_func(const Device::Pointer & device,
                         const Array::Pointer &  src,
                         Array::Pointer          small_eigenvalue,
                         Array::Pointer          middle_eigenvalue,
                         Array::Pointer          large_eigenvalue) -> std::vector<Array::Pointer>;


/**
 * @name laplace_box
 * @brief Applies the Laplace operator (Box neighborhood) to an image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'filter', 'edge detection', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_laplaceBox
 * @deprecated This function is deprecated. Consider using laplace() instead.
 */
auto
laplace_box_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name laplace_diamond
 * @brief Applies the Laplace operator (Diamond neighborhood) to an image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'filter', 'edge detection'
 * @see https://clij.github.io/clij2-docs/reference_laplaceDiamond
 * @deprecated This function is deprecated. Consider using laplace() instead.
 */
auto
laplace_diamond_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name laplace
 * @brief Applies the Laplace operator with a "box" or a "sphere" neighborhood to an image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param connectivity Filter neigborhood connectivity, "box" or "sphere" [std::string ( = "box" )]
 * @return Array::Pointer
 *
 * @note 'filter', 'edge detection'
 * @see https://clij.github.io/clij2-docs/reference_laplaceDiamond
 */
auto
laplace_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, std::string connectivity)
  -> Array::Pointer;


/**
 * @name local_cross_correlation
 * @brief Compute the cross correlation of an image to a given kernel.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param kernel Input kernel. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 * @note 'filter', 'combine', 'in assistant'
 *
 */
auto
local_cross_correlation_func(const Device::Pointer & device,
                             const Array::Pointer &  src,
                             const Array::Pointer &  kernel,
                             Array::Pointer          dst) -> Array::Pointer;


/**
 * @name logarithm
 * @brief Computes base e logarithm of all pixels values. f(x) = log(x) Author(s): Peter Haub, Robert Haase
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'filter', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_logarithm
 */
auto
logarithm_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name mask
 * @brief Computes a masked image by applying a binary mask to an image. All pixel values x of image X will be copied to
 * the destination image in case pixel value m at the same position in the mask image is not equal to zero.
 * <pre>f(x,m) = (x if (m != 0); (0 otherwise))</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param mask Mask image to apply. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @see https://clij.github.io/clij2-docs/reference_mask
 */
auto
mask_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & mask, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name mask_label
 * @brief Computes a masked image by applying a label mask to an image. All pixel values x of image X will be copied to
 * the destination image in case pixel value m at the same position in the label_map image has the right index value i.
 * <pre>f(x,m,i) = (x if (m == i); (0 otherwise))</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input Intensity image. [const Array::Pointer &]
 * @param input_labels Input Label image. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param label Label value to use. [float ( = 1 )]
 * @return Array::Pointer
 *
 * @see https://clij.github.io/clij2-docs/reference_maskLabel
 */
auto
mask_label_func(const Device::Pointer & device,
                const Array::Pointer &  src,
                const Array::Pointer &  input_labels,
                Array::Pointer          dst,
                float                   label) -> Array::Pointer;


/**
 * @name maximum_image_and_scalar
 * @brief Computes the maximum of a constant scalar s and each pixel value x in a given image X.
 * <pre>f(x, s) = max(x, s)</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param scalar Scalar value used in the comparison. [float ( = 0 )]
 * @return Array::Pointer
 *
 * @note 'filter', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_maximumImageAndScalar
 */
auto
maximum_image_and_scalar_func(const Device::Pointer & device,
                              const Array::Pointer &  src,
                              Array::Pointer          dst,
                              float                   scalar) -> Array::Pointer;


/**
 * @name maximum_images
 * @brief Computes the maximum of a pair of pixel values x, y from two given images X and Y.
 * <pre>f(x, y) = max(x, y)</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 First input image to process. [const Array::Pointer &]
 * @param src1 Second input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'combine', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_maximumImages
 */
auto
maximum_images_func(const Device::Pointer & device,
                    const Array::Pointer &  src0,
                    const Array::Pointer &  src1,
                    Array::Pointer          dst) -> Array::Pointer;


/**
 * @name maximum_box
 * @brief Computes the local maximum of a pixels cube neighborhood. The cubes size is specified by its halfwidth,
 * halfheight and halfdepth (radius).
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius size along x axis. [float ( = 1 )]
 * @param radius_y Radius size along y axis. [float ( = 1 )]
 * @param radius_z Radius size along z axis. [float ( = 1 )]
 * @return Array::Pointer
 *
 * @note 'filter', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_maximum3DBox
 * @deprecated This function is deprecated. Consider using maximum_filter() instead.
 */
auto
maximum_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 float                   radius_x,
                 float                   radius_y,
                 float                   radius_z) -> Array::Pointer;

/**
 * @name maximum_filter
 * @brief Computes the local maximum of a pixels neighborhood (box or sphere). The neighborhood size is specified by its
 * halfwidth, halfheight and halfdepth (radius).
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius size along x axis. [float ( = 1 )]
 * @param radius_y Radius size along y axis. [float ( = 1 )]
 * @param radius_z Radius size along z axis. [float ( = 1 )]
 * @param connectivity Filter neigborhood connectivity, "box" or "sphere" [std::string ( = "box" )]
 * @return Array::Pointer
 *
 * @note 'filter', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_maximum3DBox
 * @see https://clij.github.io/clij2-docs/reference_maximum3DSphere
 */
auto
maximum_filter_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z,
                    std::string             connectivity) -> Array::Pointer;

/**
 * @name grayscale_dilate
 * @brief Computes a grayscale image containing the grayscale dilation of a given input image. The
 * erosion apply the Moore neighborhood (8 pixels in 2D and 26 pixels in 3d) for the "box" connectivity and
 * the von Neumann neighborhood (4 pixels in 2D and 6 pixels in 3d) for a "sphere" connectivity. The pixels in the input
 * image with pixel value not equal to 0 will be interpreted as 1.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius size along x axis. [float ( = 1 )]
 * @param radius_y Radius size along y axis. [float ( = 1 )]
 * @param radius_z Radius size along z axis. [float ( = 1 )]
 * @param connectivity Filter neigborhood connectivity, "box" or "sphere" [std::string ( = "box" )]
 * @return Array::Pointer
 *
 * @note 'filter', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_minimum3DBox
 * @see https://clij.github.io/clij2-docs/reference_minimum3DSphere
 */
auto
grayscale_dilate_func(const Device::Pointer & device,
                      const Array::Pointer &  src,
                      Array::Pointer          dst,
                      float                   radius_x,
                      float                   radius_y,
                      float                   radius_z,
                      std::string             connectivity) -> Array::Pointer;


/**
 * @name maximum_x_projection
 * @brief Determines the maximum intensity projection of an image along X.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'projection'
 * @see https://clij.github.io/clij2-docs/reference_maximumXProjection
 */
auto
maximum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name maximum_y_projection
 * @brief Determines the maximum intensity projection of an image along X.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'projection'
 * @see https://clij.github.io/clij2-docs/reference_maximumYProjection
 */
auto
maximum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name maximum_z_projection
 * @brief Determines the maximum intensity projection of an image along Z.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'projection', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_maximumZProjection
 */
auto
maximum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name mean_box
 * @brief Computes the local mean average of a pixels boxshaped neighborhood. The cubes size is specified by its
 * halfwidth, halfheight and halfdepth (radius).
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius size along x axis. [float ( = 1 )]
 * @param radius_y Radius size along y axis. [float ( = 1 )]
 * @param radius_z Radius size along z axis. [float ( = 1 )]
 * @return Array::Pointer
 *
 * @note 'filter', 'denoise', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_mean3DBox
 * @deprecated This function is deprecated. Consider using mean() instead.
 */
auto
mean_box_func(const Device::Pointer & device,
              const Array::Pointer &  src,
              Array::Pointer          dst,
              float                   radius_x,
              float                   radius_y,
              float                   radius_z) -> Array::Pointer;


/**
 * @name mean_sphere
 * @brief Computes the local mean average of a pixels spherical neighborhood. The spheres size is specified by its
 * halfwidth, halfheight and halfdepth (radius).
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius size along x axis. [float ( = 1 )]
 * @param radius_y Radius size along y axis. [float ( = 1 )]
 * @param radius_z Radius size along z axis. [float ( = 1 )]
 * @return Array::Pointer
 *
 * @note 'filter', 'denoise', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_mean3DSphere
 * @deprecated This function is deprecated. Consider using mean() instead.
 */
auto
mean_sphere_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 float                   radius_x,
                 float                   radius_y,
                 float                   radius_z) -> Array::Pointer;

/**
 * @name mean_filter
 * @brief Computes the local mean average of a pixels neighborhood defined as a boxshaped or a sphereshaped.
 * The shape size is specified by its halfwidth, halfheight and halfdepth (radius).
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius size along x axis. [float ( = 1 )]
 * @param radius_y Radius size along y axis. [float ( = 1 )]
 * @param radius_z Radius size along z axis. [float ( = 1 )]
 * @param connectivity Filter neigborhood connectivity, "box" or "sphere" [std::string ( = "box" )]
 * @return Array::Pointer
 *
 * @note 'filter', 'denoise', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_mean3DSphere
 */
auto
mean_filter_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 float                   radius_x,
                 float                   radius_y,
                 float                   radius_z,
                 std::string             connectivity) -> Array::Pointer;


/**
 * @name mean_x_projection
 * @brief Determines the mean average intensity projection of an image along X.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'projection'
 * @see https://clij.github.io/clij2-docs/reference_meanXProjection
 */
auto
mean_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name mean_y_projection
 * @brief Determines the mean average intensity projection of an image along Y.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'projection'
 * @see https://clij.github.io/clij2-docs/reference_meanYProjection
 */
auto
mean_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name mean_z_projection
 * @brief Determines the mean average intensity projection of an image along Z.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'projection', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_meanZProjection
 */
auto
mean_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name median_box
 * @brief Computes the local median of a pixels box shaped neighborhood. The box is specified by its halfwidth and
 * halfheight (radius). For technical reasons, the area of the box must have less than 1000 pixels.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius size along x axis. [float ( = 1 )]
 * @param radius_y Radius size along y axis. [float ( = 1 )]
 * @param radius_z Radius size along z axis. [float ( = 1 )]
 * @return Array::Pointer
 *
 * @note 'filter', 'denoise', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_median3DBox
 * @deprecated This function is deprecated. Consider using median() instead.
 */
auto
median_box_func(const Device::Pointer & device,
                const Array::Pointer &  src,
                Array::Pointer          dst,
                float                   radius_x,
                float                   radius_y,
                float                   radius_z) -> Array::Pointer;


/**
 * @name median_sphere
 * @brief Computes the local median of a pixels sphere shaped neighborhood. The sphere is specified by its halfwidth and
 * halfheight (radius). For technical reasons, the area of the box must have less than 1000 pixels.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius size along x axis. [float ( = 1 )]
 * @param radius_y Radius size along y axis. [float ( = 1 )]
 * @param radius_z Radius size along z axis. [float ( = 1 )]
 * @return Array::Pointer
 *
 * @note 'filter', 'denoise', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_median3DSphere
 * @deprecated This function is deprecated. Consider using median() instead.
 */
auto
median_sphere_func(const Device::Pointer & device,
                   const Array::Pointer &  src,
                   Array::Pointer          dst,
                   float                   radius_x,
                   float                   radius_y,
                   float                   radius_z) -> Array::Pointer;

/**
 * @name median
 * @brief Computes the local median of a pixels neighborhood. The neighborhood is defined as a box or a sphere shape.
 * Its size is specified by its halfwidth, halfheight, and halfdepth (radius). For technical reasons, the area of the
 * shpae must have less than 1000 pixels.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius size along x axis. [float ( = 1 )]
 * @param radius_y Radius size along y axis. [float ( = 1 )]
 * @param radius_z Radius size along z axis. [float ( = 1 )]
 * @param connectivity Filter neigborhood connectivity, "box" or "sphere" [std::string ( = "box" )]
 * @return Array::Pointer
 *
 * @note 'filter', 'denoise', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_median3DSphere
 */
auto
median_func(const Device::Pointer & device,
            const Array::Pointer &  src,
            Array::Pointer          dst,
            float                   radius_x,
            float                   radius_y,
            float                   radius_z,
            std::string             connectivity) -> Array::Pointer;


/**
 * @name minimum_box
 * @brief Computes the local minimum of a pixels cube neighborhood. The cubes size is specified by its halfwidth,
 * halfheight and halfdepth (radius).
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius size along x axis. [float ( = 1 )]
 * @param radius_y Radius size along y axis. [float ( = 1 )]
 * @param radius_z Radius size along z axis. [float ( = 1 )]
 * @return Array::Pointer
 *
 * @note 'filter', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_minimum3DBox
 * @deprecated This function is deprecated. Consider using minimum_filter() instead.
 */
auto
minimum_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 float                   radius_x,
                 float                   radius_y,
                 float                   radius_z) -> Array::Pointer;


/**
 * @name minimum_filter
 * @brief Computes the local minimum of a pixels cube neighborhood. The cubes size is specified by its halfwidth,
 * halfheight and halfdepth (radius).
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius size along x axis. [float ( = 1 )]
 * @param radius_y Radius size along y axis. [float ( = 1 )]
 * @param radius_z Radius size along z axis. [float ( = 1 )]
 * @param connectivity Filter neigborhood connectivity, "box" or "sphere" [std::string ( = "box" )]
 * @return Array::Pointer
 *
 * @note 'filter', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_minimum3DBox
 * @see https://clij.github.io/clij2-docs/reference_minimum3DSphere
 */
auto
minimum_filter_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z,
                    std::string             connectivity) -> Array::Pointer;

/**
 * @name grayscale_erode
 * @brief Computes a grayscale image containing the grayscale erosion of a given input image. The
 * erosion apply the Mooreneighborhood (8 pixels in 2D and 26 pixels in 3d) for the "box" connectivity and
 * the vonNeumannneighborhood (4 pixels in 2D and 6 pixels in 3d) for a "sphere" connectivity. The pixels in the input
 * image with pixel value not equal to 0 will be interpreted as 1.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius size along x axis. [float ( = 1 )]
 * @param radius_y Radius size along y axis. [float ( = 1 )]
 * @param radius_z Radius size along z axis. [float ( = 1 )]
 * @param connectivity Filter neigborhood connectivity, "box" or "sphere" [std::string ( = "box" )]
 * @return Array::Pointer
 *
 * @note 'filter', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_minimum3DBox
 * @see https://clij.github.io/clij2-docs/reference_minimum3DSphere
 */
auto
grayscale_erode_func(const Device::Pointer & device,
                     const Array::Pointer &  src,
                     Array::Pointer          dst,
                     float                   radius_x,
                     float                   radius_y,
                     float                   radius_z,
                     std::string             connectivity) -> Array::Pointer;


/**
 * @name minimum_image_and_scalar
 * @brief Computes the minimum of a constant scalar s and each pixel value x in a given image X. <pre>f(x, s) = min(x,
 * s)</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param scalar Scalar value used in the comparison. [float ( = 0 )]
 * @return Array::Pointer
 *
 * @note 'filter', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_minimumImageAndScalar
 */
auto
minimum_image_and_scalar_func(const Device::Pointer & device,
                              const Array::Pointer &  src,
                              Array::Pointer          dst,
                              float                   scalar) -> Array::Pointer;


/**
 * @name minimum_images
 * @brief Computes the minimum of a pair of pixel values x, y from two given images X and Y. <pre>f(x, y) = min(x,
 * y)</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 First input image to process. [const Array::Pointer &]
 * @param src1 Second input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'combine', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_minimumImages
 */
auto
minimum_images_func(const Device::Pointer & device,
                    const Array::Pointer &  src0,
                    const Array::Pointer &  src1,
                    Array::Pointer          dst) -> Array::Pointer;


/**
 * @name minimum_x_projection
 * @brief Determines the minimum intensity projection of an image along Y.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'projection'
 * @see https://clij.github.io/clij2-docs/reference_minimumXProjection
 */
auto
minimum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name minimum_y_projection
 * @brief Determines the minimum intensity projection of an image along Y.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'projection'
 * @see https://clij.github.io/clij2-docs/reference_minimumYProjection
 */
auto
minimum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name minimum_z_projection
 * @brief Determines the minimum intensity projection of an image along Z.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'projection', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_minimumZProjection
 */
auto
minimum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


auto
minimum_of_masked_pixels_reduction_func(const Device::Pointer & device,
                                        const Array::Pointer &  src,
                                        const Array::Pointer &  mask,
                                        Array::Pointer          reduced_src,
                                        Array::Pointer          reduced_mask) -> Array::Pointer;

/**
 * @name mode_box
 * @brief Computes the local mode of a pixels box shaped neighborhood. This can be used to postprocess and locally
 * correct semantic segmentation results. The box is specified by its halfwidth and halfheight (radius). For technical
 * reasons, the intensities must lie within a range from 0 to 255. In case multiple values have maximum frequency, the
 * smallest one is returned.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius size along x axis. [float ( = 1 )]
 * @param radius_y Radius size along y axis. [float ( = 1 )]
 * @param radius_z Radius size along z axis. [float ( = 1 )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant'
 * @deprecated This function is deprecated. Consider using mode() instead.
 */
auto
mode_box_func(const Device::Pointer & device,
              const Array::Pointer &  src,
              Array::Pointer          dst,
              float                   radius_x,
              float                   radius_y,
              float                   radius_z) -> Array::Pointer;


/**
 * @name mode_sphere
 * @brief Computes the local mode of a pixels sphere shaped neighborhood. This can be used to postprocess and locally
 * correct semantic segmentation results. The sphere is specified by its halfwidth and halfheight (radius). For
 * technical reasons, the intensities must lie within a range from 0 to 255. In case multiple values have maximum
 * frequency, the smallest one is returned.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius size along x axis. [float ( = 1 )]
 * @param radius_y Radius size along y axis. [float ( = 1 )]
 * @param radius_z Radius size along z axis. [float ( = 1 )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant', 'bia-bob-suggestion'
 * @deprecated This function is deprecated. Consider using mode() instead.
 */
auto
mode_sphere_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 float                   radius_x,
                 float                   radius_y,
                 float                   radius_z) -> Array::Pointer;

/**
 * @name mode
 * @brief Computes the local mode of a pixels neighborhood. This neighborhood can be shaped as a box or a sphere.
 * This can be used to postprocess and locally correct semantic segmentation results. The shape size is specified by its
 * halfwidth, halfheight, and halfdepth (radius). For technical reasons, the intensities must lie within a range from 0
 * to 255 (uint8). In case multiple values have maximum frequency, the smallest one is returned.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius size along x axis. [float ( = 1 )]
 * @param radius_y Radius size along y axis. [float ( = 1 )]
 * @param radius_z Radius size along z axis. [float ( = 1 )]
 * @param connectivity Filter neigborhood connectivity, "box" or "sphere" [std::string ( = "box" )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant'
 */
auto
mode_func(const Device::Pointer & device,
          const Array::Pointer &  src,
          Array::Pointer          dst,
          float                   radius_x,
          float                   radius_y,
          float                   radius_z,
          std::string             connectivity) -> Array::Pointer;


/**
 * @name modulo_images
 * @brief Computes the remainder of a division of pairwise pixel values in two images
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 First input image to process. [const Array::Pointer &]
 * @param src1 Second input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 * @note 'combine'
 *
 */
auto
modulo_images_func(const Device::Pointer & device,
                   const Array::Pointer &  src0,
                   const Array::Pointer &  src1,
                   Array::Pointer          dst) -> Array::Pointer;


/**
 * @name multiply_image_and_position
 * @brief Multiplies all pixel intensities with the x, y or z coordinate, depending on specified dimension.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param dimension Dimension (0,1,2) to use in the operation. [int ( = 0 )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_multiplyImageAndCoordinate
 *
 */
auto
multiply_image_and_position_func(const Device::Pointer & device,
                                 const Array::Pointer &  src,
                                 Array::Pointer          dst,
                                 int                     dimension) -> Array::Pointer;

/**
 * @name multiply_image_and_scalar
 * @brief Multiplies all pixels value x in a given image X with a constant scalar s. <pre>f(x, s) = x * s</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src The input image to be multiplied with a constant. [const Array::Pointer &]
 * @param dst Output image where results are written into. [Array::Pointer ( = None )]
 * @param scalar The number with which every pixel will be multiplied with. [float ( = 0 )]
 * @return Array::Pointer
 *
 * @note 'filter', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_multiplyImageAndScalar
 */
auto
multiply_image_and_scalar_func(const Device::Pointer & device,
                               const Array::Pointer &  src,
                               Array::Pointer          dst,
                               float                   scalar) -> Array::Pointer;


/**
 * @name multiply_images
 * @brief Multiplies all pairs of pixel values x and y from two image X and Y. <pre>f(x, y) = x * y</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 First input image to be multiplied. [const Array::Pointer &]
 * @param src1 Second image to be multiplied. [const Array::Pointer &]
 * @param dst Output image where results are written into. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'combine', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_multiplyImages
 */
auto
multiply_images_func(const Device::Pointer & device,
                     const Array::Pointer &  src0,
                     const Array::Pointer &  src1,
                     Array::Pointer          dst) -> Array::Pointer;


/**
 * @name nan_to_num
 * @brief Copies all pixels instead those which are not a number (NaN), or positive/negative infinity which are replaced
 * by a defined new value, default 0. This function aims to work similarly as its counterpart in numpy [1]. Default
 * values for posinf and neginf may differ from numpy and even differ depending on compute hardware. It is recommended
 * to specify those values.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output image where results are written into. [Array::Pointer ( = None )]
 * @param nan Value to replace nan with. [float ( = 0 )]
 * @param posinf Value to replace +inf with. [float ( = np.nan_to_num(float('inf')) )]
 * @param neginf Value to replace -inf with. [float ( = np.nan_to_num(float('-inf')) )]
 * @return Array::Pointer
 * @see https://numpy.org/doc/stable/reference/generated/numpy.nan_to_num.html
 *
 */
auto
nan_to_num_func(const Device::Pointer & device,
                const Array::Pointer &  src,
                Array::Pointer          dst,
                float                   nan = 0,
                float                   posinf = pINF,
                float                   neginf = nINF) -> Array::Pointer;


/**
 * @name nonzero_maximum_box
 * @brief Apply a maximum filter (box shape) to the input image. The radius is fixed to 1 and pixels with value 0 are
 * ignored. Note: Pixels with 0 value in the input image will not be overwritten in the output image. Thus, the result
 * image should be initialized by copying the original image in advance.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param flag Output flag (0 or 1). [Array::Pointer]
 * @param dst Output image where results are written into. [Array::Pointer ( = None )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_nonzeroMaximumBox
 * @deprecated This function is deprecated. Consider using nonzero_maximum() instead.
 */
auto
nonzero_maximum_box_func(const Device::Pointer & device,
                         const Array::Pointer &  src,
                         Array::Pointer          flag,
                         Array::Pointer          dst) -> Array::Pointer;


/**
 * @name nonzero_maximum_diamond
 * @brief Apply a maximum filter (diamond shape) to the input image. The radius is fixed to 1 and pixels with value 0
 * are ignored. Note: Pixels with 0 value in the input image will not be overwritten in the output image. Thus, the
 * result image should be initialized by copying the original image in advance.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param flag Output flag (0 or 1). [Array::Pointer]
 * @param dst Output image where results are written into. [Array::Pointer ( = None )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_nonzeroMaximumDiamond
 * @deprecated This function is deprecated. Consider using nonzero_maximum() instead.
 */
auto
nonzero_maximum_diamond_func(const Device::Pointer & device,
                             const Array::Pointer &  src,
                             Array::Pointer          flag,
                             Array::Pointer          dst) -> Array::Pointer;

/**
 * @name nonzero_maximum
 * @brief Apply a maximum filter of a neighborhood to the input image. The neighborhood shape can be a box or a sphere.
 * The size is fixed to 1 and pixels with value 0 are ignored. Note: Pixels with 0 value in the input image will not
 * be overwritten in the output image. Thus, the result image should be initialized by copying the original image in
 * advance.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param flag Output flag (0 or 1). [Array::Pointer]
 * @param dst Output image where results are written into. [Array::Pointer ( = None )]
 * @param connectivity Filter neigborhood connectivity, "box" or "sphere" [std::string ( = "box" )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_nonzeroMaximumBox
 * @see https://clij.github.io/clij2-docs/reference_nonzeroMaximumDiamond
 *
 */
auto
nonzero_maximum_func(const Device::Pointer & device,
                     const Array::Pointer &  src,
                     Array::Pointer          flag,
                     Array::Pointer          dst,
                     std::string             connectivity) -> Array::Pointer;

/**
 * @name nonzero_minimum_box
 * @brief Apply a minimum filter (box shape) to the input image. The radius is fixed to 1 and pixels with value 0 are
 * ignored. Note: Pixels with 0 value in the input image will not be overwritten in the output image. Thus, the result
 * image should be initialized by copying the original image in advance.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param flag Output flag (0 or 1). [Array::Pointer]
 * @param dst Output image where results are written into. [Array::Pointer ( = None )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_nonzeroMinimumBox
 * @deprecated This function is deprecated. Consider using nonzero_minimum() instead.
 */
auto
nonzero_minimum_box_func(const Device::Pointer & device,
                         const Array::Pointer &  src,
                         Array::Pointer          flag,
                         Array::Pointer          dst) -> Array::Pointer;


/**
 * @name nonzero_minimum_diamond
 * @brief Apply a minimum filter (diamond shape) to the input image. The radius is fixed to 1 and pixels with value 0
 * are ignored.Note: Pixels with 0 value in the input image will not be overwritten in the output image. Thus, the
 * result image should be initialized by copying the original image in advance.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param flag Output flag (0 or 1). [Array::Pointer]
 * @param dst Output image where results are written into. [Array::Pointer ( = None )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_nonzeroMinimumDiamond
 * @deprecated This function is deprecated. Consider using nonzero_minimum() instead.
 */
auto
nonzero_minimum_diamond_func(const Device::Pointer & device,
                             const Array::Pointer &  src,
                             Array::Pointer          flag,
                             Array::Pointer          dst) -> Array::Pointer;

/**
 * @name nonzero_minimum
 * @brief Apply a minimum filter of a neighborhood to the input image. The neighborhood shape can be a box or a sphere.
 * The radius is fixed to 1 and pixels with value 0 are ignored.Note: Pixels with 0 value in the input image will not be
 * overwritten in the output image. Thus, the result image should be initialized by copying the original image in
 * advance.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param flag Output flag (0 or 1). [Array::Pointer]
 * @param dst Output image where results are written into. [Array::Pointer ( = None )]
 * @param connectivity Filter neigborhood connectivity, "box" or "sphere" [std::string ( = "box" )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_nonzeroMinimumBox
 * @see https://clij.github.io/clij2-docs/reference_nonzeroMinimumDiamond
 */
auto
nonzero_minimum_func(const Device::Pointer & device,
                     const Array::Pointer &  src,
                     Array::Pointer          flag,
                     Array::Pointer          dst,
                     std::string             connectivity) -> Array::Pointer;


/**
 * @name not_equal
 * @brief Determines if two images A and B equal pixel wise. f(a, b) = 1 if a != b; 0 otherwise.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 First image to be compared with. [const Array::Pointer &]
 * @param src1 Second image to be compared with the first. [const Array::Pointer &]
 * @param dst The resulting binary image where pixels will be 1 only if source1 [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'combine', 'binarize', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_notEqual
 */
auto
not_equal_func(const Device::Pointer & device,
               const Array::Pointer &  src0,
               const Array::Pointer &  src1,
               Array::Pointer          dst) -> Array::Pointer;


/**
 * @name not_equal_constant
 * @brief Determines if two images A and B equal pixel wise. f(a, b) = 1 if a != b; 0 otherwise.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src The image where every pixel is compared to the constant. [const Array::Pointer &]
 * @param dst The resulting binary image where pixels will be 1 only if source1 [Array::Pointer ( = None )]
 * @param constant The constant where every pixel is compared to. [float ( = 0 )]
 * @return Array::Pointer
 *
 * @note 'binarize', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_notEqualConstant
 */
auto
not_equal_constant_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float constant)
  -> Array::Pointer;


/**
 * @name paste
 * @brief Pastes an image into another image at a given position.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param destination_x Origin pixel coodinate in x to paste. [int ( = 0 )]
 * @param destination_y Origin pixel coodinate in y to paste. [int ( = 0 )]
 * @param destination_z Origin pixel coodinate in z to paste. [int ( = 0 )]
 * @return Array::Pointer
 *
 * @note 'combine', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_paste3D
 */
auto
paste_func(const Device::Pointer & device,
           const Array::Pointer &  src,
           Array::Pointer          dst,
           int                     destination_x,
           int                     destination_y,
           int                     destination_z) -> Array::Pointer;


/**
 * @name onlyzero_overwrite_maximum_box
 * @brief Apply a local maximum filter to an image which only overwrites pixels with value 0.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param flag Output flag value, 0 or 1. [Array::Pointer]
 * @param dst Output image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_onlyzeroOverwriteMaximumBox
 * @deprecated This function is deprecated. Consider using onlyzero_overwrite_maximum() instead.
 */
auto
onlyzero_overwrite_maximum_box_func(const Device::Pointer & device,
                                    const Array::Pointer &  src,
                                    Array::Pointer          flag,
                                    Array::Pointer          dst) -> Array::Pointer;


/**
 * @name onlyzero_overwrite_maximum_diamond
 * @brief Apply a local maximum filter to an image which only overwrites pixels with value 0.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param flag Output flag value, 0 or 1. [Array::Pointer]
 * @param dst Output image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_onlyzeroOverwriteMaximumDiamond
 * @deprecated This function is deprecated. Consider using onlyzero_overwrite_maximum() instead.
 */
auto
onlyzero_overwrite_maximum_diamond_func(const Device::Pointer & device,
                                        const Array::Pointer &  src,
                                        Array::Pointer          flag,
                                        Array::Pointer          dst) -> Array::Pointer;

/**
 * @name onlyzero_overwrite_maximum
 * @brief Apply a local maximum filter to an image which only overwrites pixels with value 0.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param flag Output flag value, 0 or 1. [Array::Pointer]
 * @param dst Output image. [Array::Pointer ( = None )]
 * @param connectivity Filter neigborhood connectivity, "box" or "sphere" [std::string ( = "box" )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_onlyzeroOverwriteMaximumBox
 * @see https://clij.github.io/clij2-docs/reference_onlyzeroOverwriteMaximumDiamond
 *
 */
auto
onlyzero_overwrite_maximum_func(const Device::Pointer & device,
                                const Array::Pointer &  src,
                                Array::Pointer          flag,
                                Array::Pointer          dst,
                                std::string             connectivity) -> Array::Pointer;

/**
 * @name power
 * @brief Computes all pixels value x to the power of a given exponent a. <pre>f(x, a) = x ^ a</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param exponent Exponent value. [float ( = 1 )]
 * @return Array::Pointer
 *
 * @note 'filter', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_power
 */
auto
power_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float exponent)
  -> Array::Pointer;


/**
 * @name power_images
 * @brief Calculates x to the power of y pixel wise of two images X and Y.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 First input image to process. [const Array::Pointer &]
 * @param src1 Second input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'combine', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_powerImages
 */
auto
power_images_func(const Device::Pointer & device,
                  const Array::Pointer &  src0,
                  const Array::Pointer &  src1,
                  Array::Pointer          dst) -> Array::Pointer;


/**
 * @name range
 * @brief Crops an image according to a defined range and step size.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src First input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param start_x Range starting value in x [int ( = None )]
 * @param stop_x Range stop value in x [int ( = None )]
 * @param step_x Range step value in x [int ( = None )]
 * @param start_y Range starting value in y [int ( = None )]
 * @param stop_y Range stop value in y [int ( = None )]
 * @param step_y Range step value in y [int ( = None )]
 * @param start_z Range starting value in z [int ( = None )]
 * @param stop_z Range stop value in z [int ( = None )]
 * @param step_z Range step value in z [int ( = None )]
 * @return Array::Pointer
 *
 * @note 'transform', 'in assistant'
 */
auto
range_func(const Device::Pointer & device,
           const Array::Pointer &  src,
           Array::Pointer          dst,
           int                     start_x,
           int                     stop_x,
           int                     step_x,
           int                     start_y,
           int                     stop_y,
           int                     step_y,
           int                     start_z,
           int                     stop_z,
           int                     step_z) -> Array::Pointer;


/**
 * @name read_values_from_positions
 * @brief Go to positions in a given image specified by a pointlist and read intensities of those pixels. The
 * intensities are stored in a new vector. The positions are passed as a (x,y,z) coordinate per column.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param list List of coordinate, as a 2D matrix. [const Array::Pointer &]
 * @param dst Output vector image of intensities. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 */
auto
read_values_from_positions_func(const Device::Pointer & device,
                                const Array::Pointer &  src,
                                const Array::Pointer &  list,
                                Array::Pointer          dst) -> Array::Pointer;


/**
 * @name replace_values
 * @brief Replaces integer intensities specified in a vector image. The values are passed as a vector of values.
 * The vector index represents the old intensity and the value at that position represents the new intensity.s
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param values List of intensity values to replace. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_replaceIntensities
 */
auto
replace_values_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    const Array::Pointer &  values,
                    Array::Pointer          dst) -> Array::Pointer;


/**
 * @name replace_value
 * @brief Replaces a specific intensity in an image with a given new value.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param value_to_replace Old value. [float ( = 0 )]
 * @param value_replacement New value. [float ( = 1 )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_replaceIntensity
 *
 */
auto
replace_value_func(const Device::Pointer & device,
                   const Array::Pointer &  src,
                   Array::Pointer          dst,
                   float                   value_to_replace,
                   float                   value_replacement) -> Array::Pointer;

/**
 * @name replace_intensity
 * @brief Replaces a specific intensity in an image with a given new value.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param value_to_replace Old value. [float ( = 0 )]
 * @param value_replacement New value. [float ( = 1 )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_replaceIntensity
 * @deprecated This function is deprecated. Consider using replace_value() instead.
 */
auto
replace_intensity_func(const Device::Pointer & device,
                       const Array::Pointer &  src,
                       Array::Pointer          dst,
                       float                   value_to_replace,
                       float                   value_replacement) -> Array::Pointer;

/**
 * @name replace_intensities
 * @brief Replaces integer intensities specified in a vector image. The values are passed as a vector of values.
 * The vector index represents the old intensity and the value at that position represents the new intensity.s
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param intensities List of intensities to replace, as a vector of values. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @see https://clij.github.io/clij2-docs/reference_replaceIntensities
 * @deprecated This function is deprecated. Consider using replace_values() instead.
 */
auto
replace_intensities_func(const Device::Pointer & device,
                         const Array::Pointer &  src,
                         const Array::Pointer &  intensities,
                         Array::Pointer          dst) -> Array::Pointer;

/**
 * @name maximum_sphere
 * @brief Computes the local maximum of a pixels spherical neighborhood. The spheres size is specified by its halfwidth,
 * halfheight and halfdepth (radius).
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius size along x axis. [float ( = 1 )]
 * @param radius_y Radius size along y axis. [float ( = 1 )]
 * @param radius_z Radius size along z axis. [float ( = 0 )]
 * @return Array::Pointer
 *
 * @note 'filter', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_maximum3DSphere
 * @deprecated This function is deprecated. Consider using maximum() instead.
 */
auto
maximum_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z) -> Array::Pointer;


/**
 * @name minimum_sphere
 * @brief Computes the local minimum of a pixels spherical neighborhood. The spheres size is specified by its halfwidth,
 * halfheight and halfdepth (radius).
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius size along x axis. [float ( = 1 )]
 * @param radius_y Radius size along y axis. [float ( = 1 )]
 * @param radius_z Radius size along z axis. [float ( = 1 )]
 * @return Array::Pointer
 *
 * @note 'filter', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_minimum3DSphere
 * @deprecated This function is deprecated. Consider using minimum() instead.
 */
auto
minimum_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z) -> Array::Pointer;


/**
 * @name multiply_matrix
 * @brief Multiplies two matrices with each other. Shape of matrix1 should be equal to shape of matrix2 transposed.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param matrix1 First matrix to process. [const Array::Pointer &]
 * @param matrix2 Second matrix to process. [const Array::Pointer &]
 * @param matrix_destination Output result matrix. [Array::Pointer ( = None )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_multiplyMatrix
 *
 */
auto
multiply_matrix_func(const Device::Pointer & device,
                     const Array::Pointer &  matrix1,
                     const Array::Pointer &  matrix2,
                     Array::Pointer          matrix_destination) -> Array::Pointer;


/**
 * @name reciprocal
 * @brief Computes 1/x for every pixel value This function is supposed to work similarly to its counter part in numpy
 * [1]
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'filter', 'in assistant'
 * @see https://numpy.org/doc/stable/reference/generated/numpy.reciprocal.html
 */
auto
reciprocal_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name set
 * @brief Sets all pixel values x of a given image X to a constant value v. <pre>f(x) = v</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param scalar Value to set. [float ( = 0 )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_set
 *
 */
auto
set_func(const Device::Pointer & device, const Array::Pointer & src, float scalar) -> Array::Pointer;


/**
 * @name set_column
 * @brief Sets all pixel values x of a given column in X to a constant value v.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param column_index Column index. [int ( = 0 )]
 * @param value Value to set. [float ( = 0 )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_setColumn
 *
 */
auto
set_column_func(const Device::Pointer & device, const Array::Pointer & src, int column_index, float value)
  -> Array::Pointer;


/**
 * @name set_image_borders
 * @brief Sets all pixel values at the image border to a given value.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param value Value to set. [float ( = 0 )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_setImageBorders
 *
 */
auto
set_image_borders_func(const Device::Pointer & device, const Array::Pointer & src, float value) -> Array::Pointer;


/**
 * @name set_plane
 * @brief Sets all pixel values x of a given plane in X to a constant value v.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param plane_index Plane index. [int ( = 0 )]
 * @param value Value to set. [float ( = 0 )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_setPlane
 *
 */
auto
set_plane_func(const Device::Pointer & device, const Array::Pointer & src, int plane_index, float value)
  -> Array::Pointer;


/**
 * @name set_ramp_x
 * @brief Sets all pixel values to their X coordinate.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_setRampX
 *
 */
auto
set_ramp_x_func(const Device::Pointer & device, const Array::Pointer & src) -> Array::Pointer;


/**
 * @name set_ramp_y
 * @brief Sets all pixel values to their Y coordinate.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_setRampY
 *
 */
auto
set_ramp_y_func(const Device::Pointer & device, const Array::Pointer & src) -> Array::Pointer;


/**
 * @name set_ramp_z
 * @brief Sets all pixel values to their Z coordinate.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_setRampZ
 *
 */
auto
set_ramp_z_func(const Device::Pointer & device, const Array::Pointer & src) -> Array::Pointer;


/**
 * @name set_row
 * @brief Sets all pixel values x of a given row in X to a constant value v.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param row_index [int ( = 0 )]
 * @param value [float ( = 0 )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_setRow
 *
 */
auto
set_row_func(const Device::Pointer & device, const Array::Pointer & src, int row_index, float value) -> Array::Pointer;


/**
 * @name set_nonzero_pixels_to_pixelindex
 * @brief Replaces all 0 value pixels in an image with the index of a pixel.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output image. [Array::Pointer ( = None )]
 * @param offset Offset value to start the indexing. [int ( = 1 )]
 * @return Array::Pointer
 *
 */
auto
set_nonzero_pixels_to_pixelindex_func(const Device::Pointer & device,
                                      const Array::Pointer &  src,
                                      Array::Pointer          dst,
                                      int                     offset) -> Array::Pointer;

/**
 * @name set_where_x_equals_y
 * @brief Sets all pixel values a of a given image A to a constant value v in case its coordinates x == y. Otherwise the
 * pixel is not overwritten. If you want to initialize an identity transfrom matrix, set all pixels to 0 first.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param value Value to set. [float ( = 0 )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_setWhereXequalsY
 *
 */
auto
set_where_x_equals_y_func(const Device::Pointer & device, const Array::Pointer & src, float value) -> Array::Pointer;


/**
 * @name set_where_x_greater_than_y
 * @brief Sets all pixel values a of a given image A to a constant value v in case its coordinates x > y. Otherwise the
 * pixel is not overwritten. If you want to initialize an identity transfrom matrix, set all pixels to 0 first.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param value Value to set. [float ( = 0 )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_setWhereXgreaterThanY
 *
 */
auto
set_where_x_greater_than_y_func(const Device::Pointer & device, const Array::Pointer & src, float value)
  -> Array::Pointer;


/**
 * @name set_where_x_smaller_than_y
 * @brief Sets all pixel values a of a given image A to a constant value v in case its coordinates x < y. Otherwise the
 * pixel is not overwritten. If you want to initialize an identity transfrom matrix, set all pixels to 0 first.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param value Value to set. [float ( = 0 )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_setWhereXsmallerThanY
 *
 */
auto
set_where_x_smaller_than_y_func(const Device::Pointer & device, const Array::Pointer & src, float value)
  -> Array::Pointer;


/**
 * @name sign
 * @brief Extracts the sign of pixels. If a pixel value < 0, resulting pixel value will be 1. If it was > 0, it will
 * be 1. Otherwise it will be 0. This function aims to work similarly as its counterpart in numpy [1].
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 */
auto
sign_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name smaller
 * @brief Determines if two images A and B smaller pixel wise. f(a, b) = 1 if a < b; 0 otherwise.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 First input image to process. [const Array::Pointer &]
 * @param src1 Second input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'combine', 'binarize', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_smaller
 */
auto
smaller_func(const Device::Pointer & device,
             const Array::Pointer &  src0,
             const Array::Pointer &  src1,
             Array::Pointer          dst) -> Array::Pointer;


/**
 * @name smaller_constant
 * @brief Determines if two images A and B smaller pixel wise. f(a, b) = 1 if a < b; 0 otherwise.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param constant Constant value used in the comparison. [float ( = 0 )]
 * @return Array::Pointer
 *
 * @note 'binarize', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_smallerConstant
 */
auto
smaller_constant_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float constant)
  -> Array::Pointer;


/**
 * @name smaller_or_equal
 * @brief Determines if two images A and B smaller or equal pixel wise. f(a, b) = 1 if a <= b; 0 otherwise.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 First input image to process. [const Array::Pointer &]
 * @param src1 Second input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'combine', 'binarize', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_smallerOrEqual
 */
auto
smaller_or_equal_func(const Device::Pointer & device,
                      const Array::Pointer &  src0,
                      const Array::Pointer &  src1,
                      Array::Pointer          dst) -> Array::Pointer;


/**
 * @name smaller_or_equal_constant
 * @brief Determines if two images A and B smaller or equal pixel wise. f(a, b) = 1 if a <= b; 0 otherwise.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param constant Constant value used in the comparison. [float ( = 0 )]
 * @return Array::Pointer
 *
 * @note 'binarize', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_smallerOrEqualConstant
 */
auto
smaller_or_equal_constant_func(const Device::Pointer & device,
                               const Array::Pointer &  src,
                               Array::Pointer          dst,
                               float                   constant) -> Array::Pointer;


/**
 * @name sobel
 * @brief Convolve the image with the Sobel kernel. Author(s): Ruth WhelanJeans, Robert Haase
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'filter', 'edge detection', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_sobel
 */
auto
sobel_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name square_root
 * @brief Computes the square root of each pixel.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 * @note 'filter', 'in assistant'
 *
 */
auto
square_root_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name std_z_projection
 * @brief Determines the standard deviation intensity projection of an image stack along Z.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'projection', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_standardDeviationZProjection
 */
auto
std_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name subtract_image_from_scalar
 * @brief Subtracts one image X from a scalar s pixel wise. <pre>f(x, s) = s x</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param scalar Scalar used in the subtraction. [float ( = 0 )]
 * @return Array::Pointer
 *
 * @note 'filter', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_subtractImageFromScalar
 */
auto
subtract_image_from_scalar_func(const Device::Pointer & device,
                                const Array::Pointer &  src,
                                Array::Pointer          dst,
                                float                   scalar) -> Array::Pointer;


/**
 * @name sum_reduction_x
 * @brief Takes an image and reduces it in width by factor blocksize. The new pixels contain the sum of the reduced
 * pixels. For example, given the following image and block size 4: [0, 1, 1, 0, 1, 0, 1, 1] would lead to an image [2,
 * 3]
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param blocksize Blocksize value. [int ( = 256 )]
 * @return Array::Pointer
 *
 */
auto
sum_reduction_x_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int blocksize)
  -> Array::Pointer;


/**
 * @name sum_x_projection
 * @brief Determines the sum intensity projection of an image along Z.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'projection'
 * @see https://clij.github.io/clij2-docs/reference_sumXProjection
 */
auto
sum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name sum_y_projection
 * @brief Determines the sum intensity projection of an image along Z.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'projection'
 * @see https://clij.github.io/clij2-docs/reference_sumYProjection
 */
auto
sum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name sum_z_projection
 * @brief Determines the sum intensity projection of an image along Z.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'projection', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_sumZProjection
 */
auto
sum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name transpose_xy
 * @brief Transpose X and Y axes of an image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src The input image. [const Array::Pointer &]
 * @param dst Output image where results are written into. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'transform'
 * @see https://clij.github.io/clij2-docs/reference_transposeXY
 */
auto
transpose_xy_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name transpose_xz
 * @brief Transpose X and Z axes of an image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src The input image. [const Array::Pointer &]
 * @param dst Output image where results are written into. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'transform'
 * @see https://clij.github.io/clij2-docs/reference_transposeXZ
 */
auto
transpose_xz_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name transpose_yz
 * @brief Transpose Y and Z axes of an image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src The input image. [const Array::Pointer &]
 * @param dst Output image where results are written into. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'transform'
 * @see https://clij.github.io/clij2-docs/reference_transposeYZ
 */
auto
transpose_yz_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;


/**
 * @name undefined_to_zero
 * @brief Copies all pixels instead those which are not a number (NaN) or infinity (inf), which are replaced by 0.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_undefinedToZero
 *
 */
auto
undefined_to_zero_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name variance_box
 * @brief Computes the local variance of a pixels box neighborhood. The box size is specified by its halfwidth,
 * halfheight and halfdepth (radius). If 2D images are given, radius_z will be ignored.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius size along x axis. [float ( = 1 )]
 * @param radius_y Radius size along y axis. [float ( = 1 )]
 * @param radius_z Radius size along z axis. [float ( = 1 )]
 * @return Array::Pointer
 *
 * @note 'filter', 'edge detection', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_varianceBox
 * @deprecated This function is deprecated. Consider using variance() instead.
 */
auto
variance_box_func(const Device::Pointer & device,
                  const Array::Pointer &  src,
                  Array::Pointer          dst,
                  float                   radius_x,
                  float                   radius_y,
                  float                   radius_z) -> Array::Pointer;


/**
 * @name variance_sphere
 * @brief Computes the local variance of a pixels sphere neighborhood. The sphere size is specified by its halfwidth,
 * halfheight and halfdepth (radius). If 2D images are given, radius_z will be ignored.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius size along x axis. [float ( = 1 )]
 * @param radius_y Radius size along y axis. [float ( = 1 )]
 * @param radius_z Radius size along z axis. [float ( = 1 )]
 * @return Array::Pointer
 *
 * @note 'filter', 'edge detection', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_varianceSphere
 * @deprecated This function is deprecated. Consider using variance() instead.
 */
auto
variance_sphere_func(const Device::Pointer & device,
                     const Array::Pointer &  src,
                     Array::Pointer          dst,
                     float                   radius_x,
                     float                   radius_y,
                     float                   radius_z) -> Array::Pointer;

/**
 * @name variance_filter
 * @brief Computes the local variance of a pixels neighborhood (box or sphere). The neighborhood size is specified by
 * its halfwidth, halfheight and halfdepth (radius). If 2D images are given, radius_z will be ignored.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @param radius_x Radius size along x axis. [float ( = 1 )]
 * @param radius_y Radius size along y axis. [float ( = 1 )]
 * @param radius_z Radius size along z axis. [float ( = 1 )]
 * @param connectivity Filter neigborhood connectivity, "box" or "sphere" [std::string ( = "box" )]
 * @return Array::Pointer
 *
 * @note 'filter', 'edge detection', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_varianceBox
 * @see https://clij.github.io/clij2-docs/reference_varianceSphere
 */
auto
variance_filter_func(const Device::Pointer & device,
                     const Array::Pointer &  src,
                     Array::Pointer          dst,
                     float                   radius_x,
                     float                   radius_y,
                     float                   radius_z,
                     std::string             connectivity) -> Array::Pointer;


/**
 * @name write_values_to_positions
 * @brief Takes an array of Nx3 (or Nx4 for 3D images) where the first rows are the coordinate x,y(,z) and the last row
 * is the value.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input coordinates and values. [const Array::Pointer &]
 * @param dst Output result image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 * @see https://clij.github.io/clij2-docs/reference_writeValuesToPositions
 *
 */
auto
write_values_to_positions_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name x_position_of_maximum_x_projection
 * @brief Determines an Xposition of the maximum intensity along X and writes it into the resulting image. If there are
 * multiple xslices with the same value, the smallest X will be chosen.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image stack [const Array::Pointer &]
 * @param dst altitude map [Array::Pointer ( = None )]
 * @return Array::Pointer
 * @note 'projection', 'in assistant'
 *
 */
auto
x_position_of_maximum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name x_position_of_minimum_x_projection
 * @brief Determines an Xposition of the minimum intensity along X and writes it into the resulting image. If there are
 * multiple xslices with the same value, the smallest X will be chosen.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image stack [const Array::Pointer &]
 * @param dst altitude map [Array::Pointer ( = None )]
 * @return Array::Pointer
 * @note 'projection', 'in assistant'
 *
 */
auto
x_position_of_minimum_x_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name y_position_of_maximum_y_projection
 * @brief Determines an Yposition of the maximum intensity along Y and writes it into the resulting image. If there are
 * multiple yslices with the same value, the smallest Y will be chosen.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image stack [const Array::Pointer &]
 * @param dst altitude map [Array::Pointer ( = None )]
 * @return Array::Pointer
 * @note 'projection', 'in assistant'
 *
 */
auto
y_position_of_maximum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name y_position_of_minimum_y_projection
 * @brief Determines an Yposition of the minimum intensity along Y and writes it into the resulting image. If there are
 * multiple yslices with the same value, the smallest Y will be chosen.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image stack [const Array::Pointer &]
 * @param dst altitude map [Array::Pointer ( = None )]
 * @return Array::Pointer
 * @note 'projection', 'in assistant'
 *
 */
auto
y_position_of_minimum_y_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name z_position_of_maximum_z_projection
 * @brief Determines a Zposition of the maximum intensity along Z and writes it into the resulting image. If there are
 * multiple zslices with the same value, the smallest Z will be chosen.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image stack [const Array::Pointer &]
 * @param dst altitude map [Array::Pointer ( = None )]
 * @return Array::Pointer
 * @note 'projection', 'in assistant'
 *
 */
auto
z_position_of_maximum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


/**
 * @name z_position_of_minimum_z_projection
 * @brief Determines a Zposition of the minimum intensity along Z and writes it into the resulting image. If there are
 * multiple zslices with the same value, the smallest Z will be chosen.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image stack [const Array::Pointer &]
 * @param dst altitude map [Array::Pointer ( = None )]
 * @return Array::Pointer
 * @note 'projection', 'in assistant'
 *
 */
auto
z_position_of_minimum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;


} // namespace cle::tier1

#endif // __INCLUDE_TIER1_HPP
