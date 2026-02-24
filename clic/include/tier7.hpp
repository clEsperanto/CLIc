#pragma once

#include "tier0.hpp"

/**
 * @namespace cle::tier7
 * @brief Namespace container for all Tier 7 functions.
 *        Tier 7 functions are advanced and may rely on lower-tier functions.
 */
namespace cle::tier7
{

/**
 * @name affine_transform
 * @brief Applies an affine transformation matrix to an array and returns the result.
 *  The transformation matrix must be 3×3 or 4×4, stored as a 1D array.
 *  The matrix should be row-major, i.e., the first 3 elements are the first row of the matrix.
 *  If no matrix is given, the identity matrix will be used.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to be transformed. [const Array::Pointer &]
 * @param dst Output image. [Array::Pointer ( = None )]
 * @param transform_matrix Affine transformation matrix (3×3 or 4×4). [std::vector<float> * ( = None )]
 * @param interpolate If true, bi/trilinear interpolation will be applied, if hardware allows. [bool ( = False )]
 * @param resize Automatically determines the size of the output depending on the rotation angles. [bool ( = False )]
 * @return Array::Pointer
 *
 */
auto
affine_transform_func(const Device::Pointer & device,
                      const Array::Pointer &  src,
                      Array::Pointer          dst,
                      std::vector<float> *    transform_matrix,
                      bool                    interpolate,
                      bool                    resize) -> Array::Pointer;

/**
 * @name eroded_otsu_labeling
 * @brief Segments and labels an image using blurring, Otsu thresholding, binary erosion, and
 *  masked Voronoi labeling.
 *
 *  After blurring and Otsu thresholding the image, iterative binary erosion is applied.
 *  Objects in the eroded image are labeled, and the labels are extended to fit again into
 *  the initial binary image using masked Voronoi labeling.
 *
 *  This function is similar to voronoi_otsu_labeling. It is intended to deal better with
 *  cases where labels of objects swap into each other when objects are dense. As when using
 *  Voronoi-Otsu labeling, small objects may disappear when applying this operation.
 *
 *  This function is inspired by a similar implementation in Java by Jan Brocher (Biovoxxel).
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to be transformed. [const Array::Pointer &]
 * @param dst Output label image. [Array::Pointer ( = None )]
 * @param number_of_erosions Number of erosion iterations. [int ( = 5 )]
 * @param outline_sigma Gaussian blur sigma applied before Otsu thresholding. [float ( = 2 )]
 * @return Array::Pointer
 *
 * @note 'label', 'in assistant'
 * @see https://github.com/biovoxxel/bv3dbox
 * @see https://zenodo.org/badge/latestdoi/434949702
 */
auto
eroded_otsu_labeling_func(const Device::Pointer & device,
                          const Array::Pointer &  src,
                          Array::Pointer          dst,
                          int                     number_of_erosions,
                          float                   outline_sigma) -> Array::Pointer;


/**
 * @name rigid_transform
 * @brief Translates the image by a given vector and rotates it by given angles. Angles are given in radians. To convert
 * degrees to radians, use this formula: angle_in_radians = angle_in_degrees × π / 180.0
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to be transformed. [const Array::Pointer &]
 * @param dst Output image. [Array::Pointer ( = None )]
 * @param translate_x Translation along x axis in pixels. [float ( = 0 )]
 * @param translate_y Translation along y axis in pixels. [float ( = 0 )]
 * @param translate_z Translation along z axis in pixels. [float ( = 0 )]
 * @param angle_x Rotation around x axis in radians. [float ( = 0 )]
 * @param angle_y Rotation around y axis in radians. [float ( = 0 )]
 * @param angle_z Rotation around z axis in radians. [float ( = 0 )]
 * @param centered If true, rotate image around center; otherwise, around the origin. [bool ( = True )]
 * @param interpolate If true, bi/trilinear interpolation will be applied, if hardware allows. [bool ( = False )]
 * @param resize Automatically determines the size of the output depending on the rotation angles. [bool ( = False )]
 * @return Array::Pointer
 *
 * @note 'transform', 'in assistant'
 */
auto
rigid_transform_func(const Device::Pointer & device,
                     const Array::Pointer &  src,
                     Array::Pointer          dst,
                     float                   translate_x,
                     float                   translate_y,
                     float                   translate_z,
                     float                   angle_x,
                     float                   angle_y,
                     float                   angle_z,
                     bool                    centered,
                     bool                    interpolate,
                     bool                    resize) -> Array::Pointer;


/**
 * @name rotate
 * @brief Rotates the image by given angles. Angles are given in degrees. To convert radians to degrees, use this
 * formula: angle_in_degrees = angle_in_radians × 180.0 / π
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to be rotated. [const Array::Pointer &]
 * @param dst Output image. [Array::Pointer ( = None )]
 * @param angle_x Rotation around x axis in degrees. [float ( = 0 )]
 * @param angle_y Rotation around y axis in degrees. [float ( = 0 )]
 * @param angle_z Rotation around z axis in degrees. [float ( = 0 )]
 * @param centered If true, rotate image around center; otherwise, around the origin. [bool ( = True )]
 * @param interpolate If true, bi/trilinear interpolation will be applied, if hardware allows. [bool ( = False )]
 * @param resize Automatically determines the size of the output depending on the rotation angles. [bool ( = False )]
 * @return Array::Pointer
 *
 * @note 'transform', 'in assistant'
 */
auto
rotate_func(const Device::Pointer & device,
            const Array::Pointer &  src,
            Array::Pointer          dst,
            float                   angle_x,
            float                   angle_y,
            float                   angle_z,
            bool                    centered,
            bool                    interpolate,
            bool                    resize) -> Array::Pointer;


/**
 * @name scale
 * @brief Scales the image by given factors.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to be scaled. [const Array::Pointer &]
 * @param dst Output image. [Array::Pointer ( = None )]
 * @param factor_x Scaling factor along x axis. [float ( = 1 )]
 * @param factor_y Scaling factor along y axis. [float ( = 1 )]
 * @param factor_z Scaling factor along z axis. [float ( = 1 )]
 * @param centered If true, the image will be scaled around the center of the image. [bool ( = True )]
 * @param interpolate If true, bi/trilinear interpolation will be applied. [bool ( = False )]
 * @param resize Automatically determines the output image size. [bool ( = False )]
 * @return Array::Pointer
 *
 * @note 'transform', 'in assistant'
 */
auto
scale_func(const Device::Pointer & device,
           const Array::Pointer &  src,
           Array::Pointer          dst,
           float                   factor_x,
           float                   factor_y,
           float                   factor_z,
           bool                    centered,
           bool                    interpolate,
           bool                    resize) -> Array::Pointer;


/**
 * @name translate
 * @brief Translates the image by a given vector.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to be translated. [const Array::Pointer &]
 * @param dst Output image. [Array::Pointer ( = None )]
 * @param translate_x Translation along x axis in pixels. [float ( = 0 )]
 * @param translate_y Translation along y axis in pixels. [float ( = 0 )]
 * @param translate_z Translation along z axis in pixels. [float ( = 0 )]
 * @param interpolate If true, bi/trilinear interpolation will be applied. [bool ( = False )]
 * @return Array::Pointer
 *
 * @note 'transform', 'in assistant'
 */
auto
translate_func(const Device::Pointer & device,
               const Array::Pointer &  src,
               Array::Pointer          dst,
               float                   translate_x,
               float                   translate_y,
               float                   translate_z,
               bool                    interpolate) -> Array::Pointer;

/**
 * @name deskew_x
 * @brief Deskews a volume as acquired with oblique plane light-sheet microscopy with skew in the x direction.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to be deskewed. [const Array::Pointer &]
 * @param dst Output image. [Array::Pointer ( = None )]
 * @param angle Angle in degrees. [float ( = 30 )]
 * @param voxel_size_x Voxel size in x direction. [float ( = 1.0 )]
 * @param voxel_size_y Voxel size in y direction. [float ( = 1.0 )]
 * @param voxel_size_z Voxel size in z direction. [float ( = 1.0 )]
 * @param scale_factor Downscaling factor after deskewing. [float ( = 1.0 )]
 * @return Array::Pointer
 */
auto
deskew_x_func(const Device::Pointer & device,
              const Array::Pointer &  src,
              Array::Pointer          dst,
              float                   angle,
              float                   voxel_size_x,
              float                   voxel_size_y,
              float                   voxel_size_z,
              float                   scale_factor) -> Array::Pointer;

/**
 * @name deskew_y
 * @brief Deskews a volume as acquired with oblique plane light-sheet microscopy with skew in the y direction.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to be deskewed. [const Array::Pointer &]
 * @param dst Output image. [Array::Pointer ( = None )]
 * @param angle Angle in degrees. [float ( = 30 )]
 * @param voxel_size_x Voxel size in x direction. [float ( = 1.0 )]
 * @param voxel_size_y Voxel size in y direction. [float ( = 1.0 )]
 * @param voxel_size_z Voxel size in z direction. [float ( = 1.0 )]
 * @param scale_factor Downscaling factor after deskewing. [float ( = 1.0 )]
 * @return Array::Pointer
 */
auto
deskew_y_func(const Device::Pointer & device,
              const Array::Pointer &  src,
              Array::Pointer          dst,
              float                   angle,
              float                   voxel_size_x,
              float                   voxel_size_y,
              float                   voxel_size_z,
              float                   scale_factor) -> Array::Pointer;

/**
 * @name closing_labels
 * @brief Applies a morphological closing operation to a label image. The operation consists of iterative dilation and
 * erosion of the labels. With every iteration, box and diamond/sphere structuring elements are used; thus, the
 * operation has an octagon as the structuring element. Note: This operation assumes input images are isotropic.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input label image. [const Array::Pointer &]
 * @param dst Output label image. [Array::Pointer ( = None )]
 * @param radius Radius size for the closing. [int ( = 0 )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant'
 */
auto
closing_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius) -> Array::Pointer;


/**
 * @name erode_connected_labels
 * @brief Erodes labels to a smaller size. Note: Depending on the label image and the radius,
 *  labels may disappear and labels may split into multiple islands. Thus, overlapping labels of input and output may
 *  not have the same identifier.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image to process. [const Array::Pointer &]
 * @param dst Output label image. [Array::Pointer ( = None )]
 * @param radius Erosion radius. [int ( = 1 )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant'
 */
auto
erode_connected_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius) -> Array::Pointer;

/**
 * @name opening_labels
 * @brief Applies a morphological opening operation to a label image. The operation consists of iterative erosion and
 * dilation of the labels. With every iteration, box and diamond/sphere structuring elements are used; thus, the
 * operation has an octagon as the structuring element. Note: This operation assumes input images are isotropic.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input label image. [const Array::Pointer &]
 * @param dst Output label image. [Array::Pointer ( = None )]
 * @param radius Radius size for the opening. [int ( = 0 )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant'
 */
auto
opening_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius) -> Array::Pointer;


/**
 * @name voronoi_otsu_labeling
 * @brief Labels objects directly from gray-value images. The two sigma parameters allow tuning the segmentation result.
 * Under the hood, this filter applies two Gaussian blurs, spot detection, Otsu thresholding, and Voronoi labeling.
 * The thresholded binary image is flooded using the Voronoi tessellation approach starting from the found local maxima.
 * Note: This operation assumes input images are isotropic.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input intensity image. [const Array::Pointer &]
 * @param dst Output label image. [Array::Pointer ( = None )]
 * @param spot_sigma Controls how close detected cells can be. [float ( = 2 )]
 * @param outline_sigma Controls how precisely segmented objects are outlined. [float ( = 2 )]
 * @return Array::Pointer
 *
 * @note 'label', 'in assistant', 'bia-bob-suggestion'
 * @see https://clij.github.io/clij2-docs/reference_voronoiOtsuLabeling
 * @see https://ieeexplore.ieee.org/document/4310076
 * @see https://en.wikipedia.org/wiki/Voronoi_diagram
 */
auto
voronoi_otsu_labeling_func(const Device::Pointer & device,
                           const Array::Pointer &  src,
                           Array::Pointer          dst,
                           float                   spot_sigma,
                           float                   outline_sigma) -> Array::Pointer;


} // namespace cle::tier7
