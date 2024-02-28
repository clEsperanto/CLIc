#ifndef __INCLUDE_TIER7_HPP
#define __INCLUDE_TIER7_HPP

#include "tier0.hpp"

/**
 * @namespace cle::tier7
 * @brief Namespace container for all functions of tier 7 category
 *        Tier 7 functions are advanced functions that may rely on previous tier functions
 */
namespace cle::tier7
{
/**
 * @name rigid_transform
 * @category 'transform', 'in assistant', 'bia-bob-suggestion'
 * @brief Translate the image by a given vector and rotate it by given angles. Angles are given in degrees. To convert
 * radians to degrees, use this formula: angle_in_degrees = angle_in_radians / numpy.pi * 180.0
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Image to be transformed [const Array::Pointer &]
 * @param dst Output image [Array::Pointer ( = None )]
 * @param translate_x Translation along x axis in pixels [float ( = 0 )]
 * @param translate_y Translation along y axis in pixels [float ( = 0 )]
 * @param translate_z Translation along z axis in pixels [float ( = 0 )]
 * @param angle_x Rotation around x axis in radians [float ( = 0 )]
 * @param angle_y Rotation around y axis in radians [float ( = 0 )]
 * @param angle_z Rotation around z axis in radians [float ( = 0 )]
 * @param centered If true, rotate image around center, else around the origin [bool ( = True )]
 * @param interpolate If true, bi/trilinear interpolation will be applied, if hardware allows. [bool ( = False )]
 * @param resize Automatically determines the size of the output depending on the rotation angles. [bool ( = False )]
 * @return Array::Pointer
 *
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
 * @category 'transform', 'in assistant'
 * @brief Rotate the image by given angles. Angles are given in degrees. To convert radians to degrees, use this
 * formula: angle_in_degrees = angle_in_radians / numpy.pi * 180.0
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Image to be rotated [const Array::Pointer &]
 * @param dst Output image [Array::Pointer ( = None )]
 * @param angle_x Rotation around x axis in degrees [float ( = 0 )]
 * @param angle_y Rotation around y axis in degrees [float ( = 0 )]
 * @param angle_z Rotation around z axis in degrees [float ( = 0 )]
 * @param centered If true, rotate image around center, else around the origin [bool ( = True )]
 * @param interpolate If true, bi/trilinear interpolation will be applied, if hardware allows. [bool ( = False )]
 * @param resize Automatically determines the size of the output depending on the rotation angles. [bool ( = False )]
 * @return Array::Pointer
 *
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
 * @category 'transform', 'in assistant'
 * @brief Scale the image by given factors.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Image to be scaleded [const Array::Pointer &]
 * @param dst Output image [Array::Pointer ( = None )]
 * @param factor_x Scaling along x [float ( = 1 )]
 * @param factor_y Scaling along y [float ( = 1 )]
 * @param factor_z Scaling along z [float ( = 1 )]
 * @param centered If true, the image will be scaled to the center of the image. [bool ( = True )]
 * @param interpolate If true, bi/trilinear interplation will be applied. [bool ( = False )]
 * @param resize Automatically determines output size image. [bool ( = False )]
 * @return Array::Pointer
 *
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
 * @category 'transform', 'in assistant'
 * @brief Translate the image by a given vector.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Image to be translated [const Array::Pointer &]
 * @param dst Output image [Array::Pointer ( = None )]
 * @param translate_x Tanslation along x axis in pixels [float ( = 0 )]
 * @param translate_y Tanslation along y axis in pixels [float ( = 0 )]
 * @param translate_z Tanslation along z axis in pixels [float ( = 0 )]
 * @param interpolate If true, bi/trilinear interplation will be applied. [bool ( = False )]
 * @return Array::Pointer
 *
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
 * @name closing_labels
 * @category 'label processing', 'in assistant', 'bia-bob-suggestion'
 * @brief Apply a morphological closing operation to a label image. The operation consists of iterative dilation and
 * erosion of the labels. With every iteration, box and diamond/sphere structuring elements are used and thus, the
 * operation has an octagon as structuring element. Notes * This operation assumes input images are isotropic.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input label image [const Array::Pointer &]
 * @param dst Output label image [Array::Pointer ( = None )]
 * @param radius Radius size for the closing [int ( = 0 )]
 * @return Array::Pointer
 *
 */
auto
closing_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius)
  -> Array::Pointer;


/**
 * @name opening_labels
 * @category 'label processing', 'in assistant', 'bia-bob-suggestion'
 * @brief Apply a morphological opening operation to a label image. The operation consists of iterative erosion and
 * dilation of the labels. With every iteration, box and diamond/sphere structuring elements are used and thus, the
 * operation has an octagon as structuring element. Notes * This operation assumes input images are isotropic.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input label image [const Array::Pointer &]
 * @param dst Output label image [Array::Pointer ( = None )]
 * @param radius Radius size for the opening [int ( = 0 )]
 * @return Array::Pointer
 *
 */
auto
opening_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius)
  -> Array::Pointer;


/**
 * @name voronoi_otsu_labeling
 * @priority 1
 * @category 'label', 'in assistant', 'bia-bob-suggestion'
 * @brief Labels objects directly from greyvalue images. The two sigma parameters allow tuning the segmentation result.
 * Under the hood, this filter applies two Gaussian blurs, spot detection, Otsuthresholding [2] and Voronoilabeling [3].
 * The thresholded binary image is flooded using the Voronoi tesselation approach starting from the found local maxima.
 * Notes * This operation assumes input images are isotropic.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input greyvalue image [const Array::Pointer &]
 * @param dst Output image [Array::Pointer ( = None )]
 * @param spot_sigma Controls how close detected cells can be [float ( = 2 )]
 * @param outline_sigma Controls how precise segmented objects are outlined. [float ( = 2 )]
 * @return Array::Pointer
 *
 * @link https://clij.github.io/clij2-docs/reference_voronoiOtsuLabeling
 * @link https://ieeexplore.ieee.org/document/4310076
 * @link https://en.wikipedia.org/wiki/Voronoi_diagram
 */
auto
voronoi_otsu_labeling_func(const Device::Pointer & device,
                           const Array::Pointer &  src,
                           Array::Pointer          dst,
                           float                   spot_sigma,
                           float                   outline_sigma) -> Array::Pointer;


} // namespace cle::tier7

#endif // __INCLUDE_TIER7_HPP
