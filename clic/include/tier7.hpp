#ifndef __INCLUDE_TIER7_HPP
#define __INCLUDE_TIER7_HPP

#include "tier0.hpp"

/**
 * @namespace cle::tier7
 * @brief Namespace container for all functions of tier 2 category
 *        Tier 7 functions are advanced functions that may rely on previous tier functions
 */
namespace cle::tier7
{
/**
 * @name rigid_transform
 * @brief Translate the image by a given vector and rotate it by given angles.  Angles are given in degrees. To convert
 radians to degrees, use this formula:  angle_in_degrees = angle_in_radians / numpy.pi * 180.0
 * @category 'transform', 'in assistant', 'bia-bob-suggestion'

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
 * @brief Rotate the image by given angles.  Angles are given in degrees. To convert radians to degrees, use this
 formula:  angle_in_degrees = angle_in_radians / numpy.pi * 180.0
 * @category 'transform', 'in assistant'

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
 * @brief Scale the image by given factors.
 * @category 'transform', 'in assistant'

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
 * @brief Translate the image by a given vector.
 * @category 'transform', 'in assistant'

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
 * @brief Apply a morphological closing operation to a label image.  The operation consists of iterative dilation and
 erosion of the labels. With every iteration, box and diamond/sphere structuring elements are used and thus, the
 operation has an octagon as structuring element.  Notes ----- * This operation assumes input images are isotropic.
 * @category 'label processing', 'in assistant', 'bia-bob-suggestion'

 */
auto
closing_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius)
  -> Array::Pointer;


/**
 * @name opening_labels
 * @brief Apply a morphological opening operation to a label image.  The operation consists of iterative erosion and
 dilation of the labels. With every iteration, box and diamond/sphere structuring elements are used and thus, the
 operation has an octagon as structuring element.  Notes ----- * This operation assumes input images are isotropic.
 * @category 'label processing', 'in assistant', 'bia-bob-suggestion'

 */
auto
opening_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius)
  -> Array::Pointer;


/**
 * @name voronoi_otsu_labeling
 * @brief Labels objects directly from grey-value images.  The two sigma parameters allow tuning the segmentation
 * result. Under the hood, this filter applies two Gaussian blurs, spot detection, Otsu-thresholding [2] and
 * Voronoi-labeling [3]. The thresholded binary image is flooded using the Voronoi tesselation approach starting from
 * the found local maxima.  Notes ----- * This operation assumes input images are isotropic.
 * @category 'label', 'in assistant', 'bia-bob-suggestion'
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
