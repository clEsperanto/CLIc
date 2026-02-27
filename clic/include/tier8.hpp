#pragma once

#include "tier0.hpp"

/**
 * @namespace cle::tier8
 * @brief Namespace container for all functions of Tier 8 category.
 *        Tier 8 functions are advanced functions that may rely on previous tier functions.
 */
namespace cle::tier8
{

/**
 * @name smooth_labels
 * @brief Applies a morphological opening operation to a label image and afterward
 *   fills gaps between the labels using Voronoi labeling. Finally, the result
 *   label image is masked so that all background pixels remain background pixels.
 *   Note: It is recommended to process isotropic label images.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input label image. [const Array::Pointer &]
 * @param dst Output label image. [Array::Pointer ( = None )]
 * @param radius Smoothing radius. [int ( = 0 )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant', 'bia-bob-suggestion'
 */
auto
smooth_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius) -> Array::Pointer;


/**
 * @name smooth_connected_labels
 * @brief Applies a morphological erosion and dilation of the label image with respect to
 *     the connectivity of the labels.
 *     Note: It is recommended to process isotropic label images.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input label image. [const Array::Pointer &]
 * @param dst Output label image. [Array::Pointer ( = None )]
 * @param radius Smoothing radius. [int ( = 0 )]
 * @return Array::Pointer
 *
 * @note 'label processing', 'in assistant', 'bia-bob-suggestion'
 */
auto
smooth_connected_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius) -> Array::Pointer;

/**
 * @name fft
 * @brief Performs a 1D, 2D, or 3D FFT (Fast Fourier Transform) on the input image.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image. [const Array::Pointer &]
 * @param dst Output image. [Array::Pointer ( = None )]
 * @return Array::Pointer
 */
auto
fft_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer;

/**
 * @name ifft
 * @brief Performs a 1D, 2D, or 3D IFFT (Inverse Fast Fourier Transform) on the input image.
 * The input image must be Hermitian, and the output image must be provided as the second argument.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image. [const Array::Pointer &]
 * @param dst Output image. [const Array::Pointer &]
 * @return Array::Pointer
 */
auto
ifft_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & dst) -> Array::Pointer;

/**
 * @name convolve_fft
 * @brief Performs a 1D, 2D, or 3D convolution using FFT between an input image and a kernel.
 *
 * Input image, PSF kernel, and normalization image are expected to be in the spatial domain. The function will
 * automatically pad the input image and PSF kernel to the same size as the closest smooth size and will take care of
 * the PSF kernel centering.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image. [const Array::Pointer &]
 * @param kernel Kernel image. [const Array::Pointer &]
 * @param dst Output image. [Array::Pointer ( = None )]
 * @param correlate If true, convolution with the PSF reversed. [bool ( = False )]
 * @return Array::Pointer
 */
auto
convolve_fft_func(const Device::Pointer & device,
                  const Array::Pointer &  src,
                  const Array::Pointer &  kernel,
                  Array::Pointer          dst,
                  bool                    correlate) -> Array::Pointer;

/**
 * @name deconvolve_fft
 * @brief Performs a 1D, 2D, or 3D deconvolution using FFT between an input image and a PSF kernel.
 * The deconvolution is performed using the Richardson-Lucy algorithm and requires a maximum iteration number.
 * The user can specify a normalization image (optional) and a regularization parameter (optional) to apply Total
 * Variation regularization.
 *
 * Input image, PSF kernel, and normalization image are expected to be in the spatial domain. The function will
 * automatically pad the input image and PSF kernel to the same size as the closest smooth size and will take care of
 * the PSF kernel centering.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Input image. [const Array::Pointer &]
 * @param psf Kernel image. [const Array::Pointer &]
 * @param normalization Normalization image. [const Array::Pointer ( = None )]
 * @param dst Output image. [Array::Pointer ( = None )]
 * @param iteration Maximum number of iterations. [int ( = 100 )]
 * @param regularization Regularization parameter. [float ( = 0.0 )]
 * @return Array::Pointer
 */
auto
deconvolve_fft_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    const Array::Pointer &  psf,
                    Array::Pointer          normalization,
                    Array::Pointer          dst,
                    int                     iteration,
                    float                   regularization) -> Array::Pointer;


} // namespace cle::tier8
