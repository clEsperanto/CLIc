#pragma once

#include "array.hpp"
#include "execution.hpp"
namespace cle::fft
{

/**
 * @brief Create an hermitian Array::Pointer object from a real buffer
 *
 * Hermitian buffer are buffer containing complex number stored
 * in an interleaved way (e.g. [real0, imag0, real1, imag1, ...])
 * and are shaped as [ (width/2) +1, height, depth ].
 *
 * @param input Array::Pointer
 * @return Array::Pointer
 */
Array::Pointer
create_hermitian(const Array::Pointer & input);


/**
 * @brief Get the closest smooth shape from a given input shape
 *
 * Get the next smooth number (power of 2) from a given number to insure efficient fft operations
 *
 * @param shape std::array<size_t, 3>
 * @return std::array<size_t, 3>
 */
auto inline smooth_shape(const std::array<size_t, 3> & shape) -> std::array<size_t, 3>
{
  std::array<size_t, 3> result;
  std::transform(shape.begin(), shape.end(), result.begin(), [](size_t value) { return (value > 1) ? next_smooth(value) : 1; });
  return result;
}


/**
 * @brief Get the padding shape from a given image and kernel shape
 *
 * To avoid circular calculations during convolution and/or deconvolution
 * we need to pad the image based on the kernel size
 *
 * @param image_shape std::array<size_t, 3>
 * @param kernel_shape std::array<size_t, 3>
 * @return std::array<size_t, 3>
 */
auto
fft_pad_shape(const std::array<size_t, 3> & image_shape, const std::array<size_t, 3> & kernel_shape) -> std::array<size_t, 3>;


/**
 * @brief Execute a kernel operation for fft kernel
 *
 * Template execute function for kernel to be used on fft output image
 *
 * @param name std::string
 * @param bufferA Array::Pointer
 * @param bufferB Array::Pointer
 * @param buffer_out Array::Pointer
 * @param nElements unsigned int
 * @param global_range RangeArray
 * @param local_range RangeArray
 */
auto
execOperationKernel(const Device::Pointer & device,
                    const std::string       name,
                    const Array::Pointer &  bufferA,
                    const Array::Pointer &  bufferB,
                    Array::Pointer          buffer_out,
                    const unsigned int      nElements) -> Array::Pointer;

/**
 * @brief Execute remove small values kernel (inplace)
 *
 * Execute remove small values kernel (inplace), to be used in other fft operations
 *
 * @param BufferA Array::Pointer
 * @param nElements unsigned int
 * @param global_range RangeArray
 * @param local_range RangeArray
 */
auto
execRemoveSmallValues(const Device::Pointer & device, Array::Pointer buffer, const unsigned int nElements) -> void;

/**
 * @brief Execute a total variation term
 *
 * Execute total variation term calculation, to be used in the deconvolution operation
 *
 * @param estimate Array::Pointer
 * @param correction Array::Pointer
 * @param variation Array::Pointer
 * @param nx unsigned int
 * @param ny unsigned int
 * @param nz unsigned int
 * @param hx float
 * @param hy float
 * @param hz float
 * @param regularization_factor float
 * @param global_range RangeArray
 * @param local_range RangeArray
 */
auto
execTotalVariationTerm(const Device::Pointer & device,
                       const Array::Pointer &  estimate,
                       const Array::Pointer &  correction,
                       Array::Pointer          variation,
                       float                   hx,
                       float                   hy,
                       float                   hz,
                       float                   regularization_factor) -> void;


/**
 * @brief Fast Fourier Transform (vkFFT)
 *
 * Perform a forward FFT on a real buffer and store the result in a complex hermitian buffer
 * This operation takes a real array and returns a complex array in the frequency domain.
 * The input shape can have an impact on the computation time, it is recommended to use
 * a shape with a power of 2 for each dimension.
 *
 * @param input Array::Pointer
 * @param output Array::Pointer
 * @return Array::Pointer
 */
auto
performFFT(const Array::Pointer & input, Array::Pointer output) -> Array::Pointer;


/**
 * @brief Inverse Fast Fourier Transform (vkFFT)
 *
 * Perform a backward FFT on a complex hermitian buffer and store the result in a real buffer
 * This operation takes a complex array and returns a real array in the spatial domain.
 * The real output buffer must be provided as the second argument as the operation requires the
 * output buffer shape to be known.
 *
 * @param input Array::Pointer
 * @param output Array::Pointer
 */
auto
performIFFT(const Array::Pointer & input, const Array::Pointer & output) -> void;


/**
 * @brief FFT Convolution operation (vkFFT)
 *
 * Perform a convolution operation on two arrays in the frequency domain
 * It takes two real arrays, performs a forward FFT on both, multiply them in the frequency domain
 * and then perform a backward FFT on the result
 *
 * Caution, this operation expect the kernel to be centered at (0, 0) and the same shape as the input
 *
 * @param input Array::Pointer
 * @param psf Array::Pointer
 * @param output Array::Pointer
 * @param correlate bool
 * @return Array::Pointer
 */
auto
performConvolution(const Array::Pointer & input, const Array::Pointer & psf, const Array::Pointer & output, bool correlate) -> void;


/**
 * @brief Richardson-Lucy deconvolution (vkFFT)
 *
 * Perform a Richardson-Lucy deconvolution on an observed image using a PSF kernel
 * with optional total variation regularization term.
 *
 * Caution, this operation expect the kernel to be centered at (0, 0) and the same shape as the input
 *
 * @param observe Array::Pointer
 * @param psf Array::Pointer
 * @param normal Array::Pointer
 * @param estimate Array::Pointer
 * @param iterations size_t
 * @param regularization float
 * @return Array::Pointer
 */
auto
performDeconvolution(const Array::Pointer & observe,
                     const Array::Pointer & psf,
                     Array::Pointer         normal,
                     Array::Pointer         estimate,
                     size_t                 iterations,
                     float                  regularization) -> Array::Pointer;


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
 * @name convolve
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
convolve_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & kernel, Array::Pointer dst, bool correlate)
  -> Array::Pointer;

/**
 * @name deconvolve
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
deconvolve_func(const Device::Pointer & device,
                const Array::Pointer &  src,
                const Array::Pointer &  psf,
                Array::Pointer          normalization,
                Array::Pointer          dst,
                int                     iteration,
                float                   regularization) -> Array::Pointer;


} // namespace cle::fft
