#ifndef __INCLUDE_FFT_HPP
#define __INCLUDE_FFT_HPP

// #include "clFFT.h"

#include "vkFFT.h"

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
 * @param real_buf Array::Pointer
 * @return Array::Pointer
 */
Array::Pointer
create_hermitian(const Array::Pointer & real_buf);

/**
 * @brief Get the next smooth shape from a given shape
 *
 * Get the next smooth number from a given number
 *
 * @param shape std::array<size_t, 3>
 * @return std::array<size_t, 3>
 */
auto
get_next_smooth(const std::array<size_t, 3> & shape) -> std::array<size_t, 3>;

/**
 * @brief Execute operation for fft kernel
 *
 * Template execute function for fft kernel, to be used in other fft operations
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
performIFFT(const Array::Pointer & input, Array::Pointer output) -> void;

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
performConvolution(const Array::Pointer & input, const Array::Pointer & psf, const Array::Pointer & output, bool correlate)
  -> void;

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


} // namespace cle::fft

#endif // __INCLUDE_FFT_HPP
