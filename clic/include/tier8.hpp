#ifndef __INCLUDE_TIER8_HPP
#define __INCLUDE_TIER8_HPP

#include "tier0.hpp"

/**
 * @namespace cle::tier8
 * @brief Namespace container for all functions of tier 8 category
 *        Tier 8 functions are advanced functions that may rely on previous tier functions
 */
namespace cle::tier8
{

/**
 * @name smooth_labels
 * @brief Apply a morphological opening operation to a label image and afterwards
 *   fills gaps between the labels using voronoi-labeling. Finally, the result
 *   label image is masked so that all background pixels remain background pixels.
 *
 *   Note: It is recommended to process isotropic label images.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input label image [const Array::Pointer &]
 * @param dst Output label image [Array::Pointer ( = None )]
 * @param radius Smoothing radius.[int ( = 0 )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant', 'bia-bob-suggestion'
 */
auto
smooth_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius)
  -> Array::Pointer;


/**
 * @name smooth_connected_labels
 * @brief Apply a morphological erosion and dilation of the label image with respect to
 *     the connectivity of the labels.
 *
 *   Note: It is recommended to process isotropic label images.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input label image [const Array::Pointer &]
 * @param dst Output label image [Array::Pointer ( = None )]
 * @param radius Smoothing radius. [int ( = 0 )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant', 'bia-bob-suggestion'
 */
auto
smooth_connected_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius)
  -> Array::Pointer;

/**
 * @name fft
 * @brief Perform a 1D, 2D or 3D FFT on the input image
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image [const Array::Pointer &]
 * @param dst Output image [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'fft', 'in assistant', 'bia-bob-suggestion'
 */
auto
fft_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

/**
 * @name ifft
 * @brief Perform a 1D, 2D or 3D IFFT (Inverse FFT) on the input image (must be of complex type)
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image [const Array::Pointer &]
 * @param dst Output image [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'fft', 'in assistant', 'bia-bob-suggestion'
 */
auto
ifft_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

/**
 * @name fft_convolution
 * @brief Perform a 1D, 2D or 3D convolution using FFT between an input image and a kernel
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image [const Array::Pointer &]
 * @param kernel Kernel image [const Array::Pointer &]
 * @param dst Output image [Array::Pointer ( = None )]
 * @param correlate If true, perform correlation instead of convolution. [bool ( = false )]
 * @return Array::Pointer
 *
 * @note 'fft', 'in assistant', 'bia-bob-suggestion'
 */
auto
fft_convolution_func(const Device::Pointer & device,
                     const Array::Pointer &  src,
                     const Array::Pointer &  kernel,
                     Array::Pointer          dst,
                     bool                    correlate) -> Array::Pointer;

auto
fft_deconvolution_func(const Device::Pointer & device,
                       const Array::Pointer &  src,
                       const Array::Pointer &  psf,
                       Array::Pointer          normalization,
                       Array::Pointer          dst,
                       size_t                  interation,
                       float                   regularization) -> Array::Pointer;


} // namespace cle::tier8

#endif // __INCLUDE_TIER8_HPP
