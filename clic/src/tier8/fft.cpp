#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"
#include "tier6.hpp"
#include "tier7.hpp"
#include "tier8.hpp"

#include "fft.hpp"

#include "utils.hpp"

#include <iostream>

namespace cle::tier8
{

auto
fft_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return fft::performFFT(src, dst);
}

auto
ifft_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  if (src->dtype() != dType::COMPLEX)
  {
    std::cerr << "Warning: ifft input buffer expect COMPLEX type, but got " << toString(src->dtype()) << "."
              << std::endl;
  }
  if (dst == nullptr)
  {
    throw std::runtime_error("Error: ifft output buffer is null. Please provide an output buffer.");
  }

  fft::performIFFT(src, dst);

  return dst;
}

auto
convolve_fft_func(const Device::Pointer & device,
                  const Array::Pointer &  src,
                  const Array::Pointer &  kernel,
                  Array::Pointer          dst,
                  bool                    correlate) -> Array::Pointer
{
  if (kernel->width() > src->width() || kernel->height() > src->height() || kernel->depth() > src->depth())
  {
    std::ostringstream oss;
    oss << "Error: Kernel size is larger than the input buffer size. Kernel size: " << kernel->width() << "x"
        << kernel->height() << "x" << kernel->depth() << " Input size: " << src->width() << "x" << src->height() << "x"
        << src->depth();
    throw std::runtime_error(oss.str());
  }

  const RangeArray image_shape = { src->width(), src->height(), src->depth() };
  const RangeArray kernel_shape = { kernel->width(), kernel->height(), kernel->depth() };

  // get the next smooth shape for the input and kernel to facilitate the fft
  auto pad_shape = fft::fft_pad_shape(image_shape, kernel_shape);
  auto smoothed_shape = fft::fft_smooth_shape(pad_shape);

  // check if smooth size differs from the input size, if yes pad input and save the padding size for unpadding
  bool           padded = false;
  Array::Pointer pad_input = src;
  if (smoothed_shape[0] != image_shape[0] || smoothed_shape[1] != image_shape[1] || smoothed_shape[2] != image_shape[2])
  {
    pad_input = tier1::pad_func(device, src, nullptr, smoothed_shape[0], smoothed_shape[1], smoothed_shape[2], 0, true);
    padded = true;
  }

  // check if smooth size differs from the kernel size, if yes, pad the kernel
  Array::Pointer pad_kernel = kernel;
  if (smoothed_shape[0] != kernel_shape[0] || smoothed_shape[1] != kernel_shape[1] || smoothed_shape[2] != kernel_shape[2])
  {
    pad_kernel = tier1::pad_func(device, kernel, nullptr, smoothed_shape[0], smoothed_shape[1], smoothed_shape[2], 0, true);
  }

  // check dst size and pad if needed, otherwise create a new buffer
  // negative shift kernel to center it at (0, 0, 0)
  auto x_center = (static_cast<int>(std::ceil(pad_kernel->width() / 2.0)) - 1);
  auto y_center = (static_cast<int>(std::ceil(pad_kernel->height() / 2.0)) - 1);
  auto z_center = (static_cast<int>(std::ceil(pad_kernel->depth() / 2.0)) - 1);
  pad_kernel = tier1::circular_shift_func(device, pad_kernel, nullptr, -x_center, -y_center, -z_center);

  // perform convolution
  tier0::create_like(pad_input, dst);
  fft::performConvolution(pad_input, pad_kernel, dst, correlate);

  // unpad the result if needed
  if (padded)
  {
    dst = tier1::unpad_func(device, dst, nullptr, image_shape[0], image_shape[1], image_shape[2], true);
  }

  return dst;
}

auto
deconvolve_fft_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    const Array::Pointer &  psf,
                    Array::Pointer          normalization,
                    Array::Pointer          dst,
                    size_t                  iteration,
                    float                   regularization) -> Array::Pointer
{
  if (psf->width() > src->width() || psf->height() > src->height() || psf->depth() > src->depth())
  {
    std::ostringstream oss;
    oss << "Error: Kernel size is larger than the input buffer size. Kernel size: " << psf->width() << "x"
        << psf->height() << "x" << psf->depth() << " Input size: " << src->width() << "x" << src->height() << "x"
        << src->depth();
    throw std::runtime_error(oss.str());
  }
  
  const RangeArray image_shape = { src->width(), src->height(), src->depth() };
  const RangeArray psf_shape = { psf->width(), psf->height(), psf->depth() };

  // auto pad_shape = fft::fft_pad_shape(image_shape, psf_shape);
  RangeArray pad_shape = {image_shape[0], image_shape[1], image_shape[2]};
  auto smoothed_shape = fft::fft_smooth_shape(pad_shape);

  // check if smooth size differs from the input size, if yes pad input and save the padding size for unpadding
  bool           padded = false;
  Array::Pointer pad_input = src;
  if (smoothed_shape[0] != image_shape[0] || smoothed_shape[1] != image_shape[1] || smoothed_shape[2] != image_shape[2])
  {
    pad_input = tier1::pad_func(device, src, nullptr, smoothed_shape[0], smoothed_shape[1], smoothed_shape[2], 0, true);
    padded = true;
  }

  // check if smooth size differs from the kernel size, if yes, pad the kernel
  Array::Pointer pad_psf = psf;
  if (smoothed_shape[0] != psf_shape[0] || smoothed_shape[1] != psf_shape[1] || smoothed_shape[2] != psf_shape[2])
  {
    pad_psf = tier1::pad_func(device, psf, nullptr, smoothed_shape[0], smoothed_shape[1], smoothed_shape[2], 0, true);
  }

  // check if smooth size differs from the kernel size, if yes, pad the kernel
  Array::Pointer pad_norm = normalization;
  if (pad_norm != nullptr)
  {
    const RangeArray norm_shape = { normalization->width(), normalization->height(), normalization->depth() };
    if (smoothed_shape[0] != norm_shape[0] || smoothed_shape[1] != norm_shape[1] || smoothed_shape[2] != norm_shape[2])

    {
      pad_norm = tier1::pad_func(device, normalization, nullptr, smoothed_shape[0], smoothed_shape[1], smoothed_shape[2], 0, true);
    }
  }

  // check dst size and pad if needed, otherwise create a new buffer
  Array::Pointer pad_dst = dst;
  if (pad_dst != nullptr)
  {
    const RangeArray dst_shape = { dst->width(), dst->height(), dst->depth() };
    if (smoothed_shape[0] != dst_shape[0] || smoothed_shape[1] != dst_shape[1] || smoothed_shape[2] != dst_shape[2])
    {
      pad_dst = tier1::pad_func(device, pad_dst, nullptr, smoothed_shape[0], smoothed_shape[1], smoothed_shape[2], 0, true);
    }
  }
  else
  {
    pad_dst = Array::create(pad_input);
    pad_dst->fill(1);
    // pad_input->copyTo(pad_dst);
  }

  // shift kenerl to center it at (0, 0), -1 because we use 0-based index
  auto x_center = (static_cast<int>(pad_psf->width()) / 2) - 1;
  auto y_center = (static_cast<int>(pad_psf->height()) / 2) - 1;
  auto z_center = (static_cast<int>(pad_psf->depth()) / 2) - 1;
  pad_psf = tier1::circular_shift_func(device, pad_psf, nullptr, x_center, y_center, z_center);

  // perform deconvolution§
  fft::performDeconvolution(pad_input, pad_psf, pad_norm, pad_dst, iteration, regularization);

  // unpad the result if needed
  if (padded)
  {
    dst = tier1::unpad_func(device, pad_dst, nullptr, image_shape[0], image_shape[1], image_shape[2], true);
  }
  else
  {
    dst = pad_dst;
  }

  return dst;
}

} // namespace cle::tier8
