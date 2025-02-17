#ifndef __INCLUDE_FFT_HPP
#define __INCLUDE_FFT_HPP

#include "clFFT.h"

#include "array.hpp"
namespace cle::fft
{

Array::Pointer create_hermitian(const Array::Pointer & real_buf);

auto SetupFFT() -> cl_int;

auto bake_forward(const Array::Pointer & real) -> clfftPlanHandle;

auto bake_backward(const Array::Pointer & real) -> clfftPlanHandle;

auto execOperationKernel(const std::string name, const Array::Pointer & bufferA, const Array::Pointer & bufferB, const Array::Pointer & buffer_out, size_t nElements, const RangeArray& global_range, const RangeArray& local_range) -> void;

auto execRemoveSmallValues(Array::Pointer & BufferA, size_t nElements, const RangeArray& global_range, const RangeArray& local_range) -> void;

auto execTotalVariationTerm(const Array::Pointer & estimate, const Array::Pointer & correction, const Array::Pointer & variation, size_t nx, size_t ny, size_t nz, float hx, float hy, float hz, float regularization_factor, const RangeArray& global_range, const RangeArray& local_range) -> void;

auto fft_forward(const Array::Pointer & real, Array::Pointer complex) -> void;

auto fft_backward(const Array::Pointer & complex, Array::Pointer real) -> void;

auto fft_execute(const Array::Pointer & data) -> Array::Pointer;

auto conv(const Array::Pointer & input, const Array::Pointer & psf, Array::Pointer output) -> Array::Pointer;

auto deconv(const Array::Pointer & input, const Array::Pointer & psf, Array::Pointer normal, Array::Pointer output, size_t iterations) -> Array::Pointer;

} // namespace cle::fft

#endif // __INCLUDE_FFT_HPP
