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
 * @param src target image [const Array::Pointer &]
 * @param dst translation along x axis in pixels [Array::Pointer ( = None )]
 * @param translate_x translation along y axis in pixels [float ( = 0 )]
 * @param translate_y translation along z axis in pixels [float ( = 0 )]
 * @param translate_z rotation around x axis in radians [float ( = 0 )]
 * @param angle_x rotation around y axis in radians [float ( = 0 )]
 * @param angle_y rotation around z axis in radians [float ( = 0 )]
 * @param angle_z [float ( = 0 )]
 * @param centered [bool ( = True )]
 * @param interpolate [bool ( = False )]
 * @param resize If true, bi/trilinear interpolation will be applied, if hardware allows. [bool ( = False )]
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
 * @param src target image [const Array::Pointer &]
 * @param dst rotation around x axis in degrees [Array::Pointer ( = None )]
 * @param angle_x rotation around y axis in degrees [float ( = 0 )]
 * @param angle_y rotation around z axis in degrees [float ( = 0 )]
 * @param angle_z [float ( = 0 )]
 * @param centered [bool ( = True )]
 * @param interpolate [bool ( = False )]
 * @param resize If true, bi/trilinear interpolation will be applied, if hardware supports it. [bool ( = False )]
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
 * @param src target image [const Array::Pointer &]
 * @param dst scaling along x [Array::Pointer ( = None )]
 * @param factor_x scaling along y [float ( = 1 )]
 * @param factor_y scaling along z [float ( = 1 )]
 * @param factor_z If true, the image will be scaled to the center of the image. [float ( = 1 )]
 * @param centered If true, bi/trilinear interplation will be applied. [bool ( = True )]
 * @param interpolate Automatically determines output size image. [bool ( = False )]
 * @param resize [bool ( = False )]
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
 * @param src image to be translated [const Array::Pointer &]
 * @param dst target image [Array::Pointer ( = None )]
 * @param translate_x translation along x axis in pixels [float ( = 0 )]
 * @param translate_y translation along y axis in pixels [float ( = 0 )]
 * @param translate_z translation along z axis in pixels [float ( = 0 )]
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
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @param radius [int ( = 0 )]
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
 * @param src [const Array::Pointer &]
 * @param dst [Array::Pointer ( = None )]
 * @param radius [int ( = 0 )]
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
 * @param spot_sigma controls how close detected cells can be [float ( = 2 )]
 * @param outline_sigma controls how precise segmented objects are outlined. [float ( = 2 )]
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
