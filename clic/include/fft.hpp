#ifndef __INCLUDE_FFT_HPP
#define __INCLUDE_FFT_HPP

#include "clFFT.h"

#include "vkFFT.h"

#include "array.hpp"
#include "execution.hpp"
namespace cle::fft
{

/**
 * @brief Create a new hermitian Array::Pointer object from a real buffer
 *
 * Hermitian buffer are buffer containing the real and imaginary part of a complex number
 * in an interleaved way (e.g. [real0, imag0, real1, imag1, ...])
 *
 * @param real_buf Array::Pointer
 * @return Array::Pointer
 */
Array::Pointer
create_hermitian(const Array::Pointer & real_buf);

/**
 * @brief Setup the clFFT library
 *
 * Setup procedure before using the clFFT library
 *
 * @return cl_int
 */
auto
SetupFFT() -> cl_int;

/**
 * @brief Create a forward plan for the clFFT library
 *
 * Create a forward plan for the clFFT library
 *
 * @param real Array::Pointer
 * @return clfftPlanHandle
 */
auto
bake_forward(const Array::Pointer & real) -> clfftPlanHandle;


/**
 * @brief Create a backward plan for the clFFT library
 *
 * Create a backward plan for the clFFT library
 *
 * @param real Array::Pointer
 * @return clfftPlanHandle
 */
auto
bake_backward(const Array::Pointer & real) -> clfftPlanHandle;


/**
 * @brief Execute operation for fft kernel
 *
 * Template execute function for fft kernel
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
execOperationKernel(const std::string      name,
                    const Array::Pointer & bufferA,
                    const Array::Pointer & bufferB,
                    Array::Pointer  buffer_out,
                    unsigned int           nElements,
                    const RangeArray &     global_range,
                    const RangeArray &     local_range) -> void;

/**
 * @brief Execute remove small values kernel (inplace)
 *
 * Execute remove small values kernel (inplace)
 *
 * @param BufferA Array::Pointer
 * @param nElements unsigned int
 * @param global_range RangeArray
 * @param local_range RangeArray
 */
auto
execRemoveSmallValues(Array::Pointer   BufferA,
                      unsigned int       nElements,
                      const RangeArray & global_range,
                      const RangeArray & local_range) -> void;

/**
 * @brief Execute a total variation term
 *
 * Execute total variation term calculation
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
execTotalVariationTerm(const Array::Pointer & estimate,
                       const Array::Pointer & correction,
                       Array::Pointer variation,
                       unsigned int           nx,
                       unsigned int           ny,
                       unsigned int           nz,
                       float                  hx,
                       float                  hy,
                       float                  hz,
                       float                  regularization_factor,
                       const RangeArray &     global_range,
                       const RangeArray &     local_range) -> void;


/**
 * @brief Fast Fourier Transform (vkFFT)
 *
 * Perform a forward FFT on a real buffer and store the result in a complex hermitian buffer
 * This operation takes a real array and returns a complex array in the frequency domain.
 * If provided, will store the result in the provided complex buffer otherwise will create a new one
 * and return it.
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
 * The real output buffer must be provided as the second argument.
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
 * @param input Array::Pointer
 * @param psf Array::Pointer
 * @param output Array::Pointer
 * @param correlate bool
 * @return Array::Pointer
 */
auto
performConvolution(const Array::Pointer & input, const Array::Pointer & psf, Array::Pointer output, bool correlate)
  -> Array::Pointer;

/**
 * @brief Richardson Lucy deconvolution (vkFFT)
 *
 * Perform a Richardson Lucy deconvolution on an observed image using a PSF kernel
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
 * @brief Fast Fourier Transform (clFFT)
 *
 * Perform a forward FFT on a real buffer and store the result in a complex buffer
 * This operation takes a real array and returns a complex array in the frequency domain
 *
 * @param real Array::Pointer
 * @param complex Array::Pointer
 * @return Array::Pointer
 */
auto
fft_forward(const Array::Pointer & real, Array::Pointer complex) -> Array::Pointer;

/**
 * @brief Inverse Fast Fourier Transform (clFFT)
 *
 * Perform a backward FFT on a complex buffer and store the result in a real buffer
 * This operation takes a complex array and returns a real array in the spatial domain
 *
 * @param complex Array::Pointer
 * @param real Array::Pointer
 */
auto
fft_backward(const Array::Pointer & complex, Array::Pointer real) -> void;

/**
 * @brief FFT Convolution operation
 *
 * Perform a convolution operation on two arrays in the frequency domain
 * It takes two real arrays, performs a forward FFT on both, multiply them in the frequency domain
 * and then perform a backward FFT on the result
 *
 * @param input Array::Pointer
 * @param psf Array::Pointer
 * @param output Array::Pointer
 * @param correlate bool
 * @return Array::Pointer
 */
auto
convolution(const Array::Pointer & input, const Array::Pointer & psf, Array::Pointer output, bool correlate)
  -> Array::Pointer;

/**
 * @brief Richardson Lucy deconvolution
 *
 * Perform a Richardson Lucy deconvolution on an observed image using a PSF kernel
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
deconvolution(const Array::Pointer & observe,
              const Array::Pointer & psf,
              Array::Pointer         normal,
              Array::Pointer         estimate,
              size_t                 iterations,
              float                  regularization) -> Array::Pointer;

} // namespace cle::fft

#endif // __INCLUDE_FFT_HPP
