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

  // get the next smooth shape for the input and kernel to facilitate the fft
  auto smoothed_shape = fft::get_next_smooth({ src->width(), src->height(), src->depth() });

  // check if smooth size differs from the input size, if yes pad input and save the padding size for unpadding
  bool           padded = false;
  Array::Pointer pad_input = nullptr;
  auto           input_pad_x = src->width();
  auto           input_pad_y = src->height();
  auto           input_pad_z = src->depth();
  if (smoothed_shape[0] != src->width() || smoothed_shape[1] != src->height() || smoothed_shape[2] != src->depth())
  {
    // pad src to the smooth size
    input_pad_x = static_cast<int>(smoothed_shape[0]) - static_cast<int>(input_pad_x);
    input_pad_y = static_cast<int>(smoothed_shape[1]) - static_cast<int>(input_pad_y);
    input_pad_z = static_cast<int>(smoothed_shape[2]) - static_cast<int>(input_pad_z);
    pad_input = tier1::pad_func(device, src, nullptr, input_pad_x, input_pad_y, input_pad_z, 0, true);
    padded = true;
  }
  else
  {
    pad_input = src;
  }

  // check if smooth size differs from the kernel size, if yes, pad the kernel
  Array::Pointer pad_kernel = kernel;
  if (smoothed_shape[0] != kernel->width() || smoothed_shape[1] != kernel->height() ||
      smoothed_shape[2] != kernel->depth())
  {
    // pad kernel to the smooth size
    auto kernel_pad_x = static_cast<int>(smoothed_shape[0]) - static_cast<int>(kernel->width());
    auto kernel_pad_y = static_cast<int>(smoothed_shape[1]) - static_cast<int>(kernel->height());
    auto kernel_pad_z = static_cast<int>(smoothed_shape[2]) - static_cast<int>(kernel->depth());
    pad_kernel = tier1::pad_func(device, kernel, nullptr, kernel_pad_x, kernel_pad_y, kernel_pad_z, 0, true);
  }
  // check dst size and pad if needed, otherwise create a new buffer
  // negative shift kernel to center it at (0, 0, 0)
  auto x_center = (static_cast<int>(std::ceil(pad_kernel->width() / 2.0)) - 1);
  auto y_center = (static_cast<int>(std::ceil(pad_kernel->height() / 2.0)) - 1);
  auto z_center = (static_cast<int>(std::ceil(pad_kernel->depth() / 2.0)) - 1);
  pad_kernel = tier1::circular_shift_func(device, pad_kernel, nullptr, -x_center, -y_center, -z_center);

  // perform convolution
  auto padded_dst = fft::performConvolution(pad_input, pad_kernel, nullptr, correlate);

  // unpad the result if needed
  if (padded)
  {
    tier1::unpad_func(device, padded_dst, dst, input_pad_x, input_pad_y, input_pad_z, true);
  }
  else
  {
    dst = padded_dst;
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

  auto smoothed_shape = fft::get_next_smooth({ src->width(), src->height(), src->depth() });

  // check if smooth size differs from the input size, if yes pad input and save the padding size for unpadding
  bool           padded = false;
  Array::Pointer pad_input = src;
  size_t         input_pad_size_x = src->width();
  size_t         input_pad_size_y = src->height();
  size_t         input_pad_size_z = src->depth();
  if (smoothed_shape[0] != src->width() || smoothed_shape[1] != src->height() || smoothed_shape[2] != src->depth())
  {
    // pad src to the smooth size
    input_pad_size_x -= smoothed_shape[0];
    input_pad_size_y -= smoothed_shape[1];
    input_pad_size_z -= smoothed_shape[2];
    pad_input = tier1::pad_func(device, src, nullptr, input_pad_size_x, input_pad_size_y, input_pad_size_z, 0, true);
    padded = true;
  }

  // check if smooth size differs from the kernel size, if yes, pad the kernel
  Array::Pointer pad_psf = psf;
  if (smoothed_shape[0] != psf->width() || smoothed_shape[1] != psf->height() || smoothed_shape[2] != psf->depth())
  {
    // pad kernel to the smooth size
    auto pad_size_x = smoothed_shape[0] - psf->width();
    auto pad_size_y = smoothed_shape[1] - psf->height();
    auto pad_size_z = smoothed_shape[2] - psf->depth();

    pad_psf = tier1::pad_func(device, psf, nullptr, pad_size_x, pad_size_y, pad_size_z, 0, true);
  }

  // check if smooth size differs from the kernel size, if yes, pad the kernel
  Array::Pointer pad_norm = normalization;
  if (pad_norm != nullptr)
  {
    if (smoothed_shape[0] != normalization->width() || smoothed_shape[1] != normalization->height() ||
        smoothed_shape[2] != normalization->depth())
    {
      // pad kernel to the smooth size
      auto pad_size_x = smoothed_shape[0] - normalization->width();
      auto pad_size_y = smoothed_shape[1] - normalization->height();
      auto pad_size_z = smoothed_shape[2] - normalization->depth();
      pad_norm = tier1::pad_func(device, normalization, nullptr, pad_size_x, pad_size_y, pad_size_z, 0, true);
    }
  }

  // check dst size and pad if needed, otherwise create a new buffer
  Array::Pointer pad_dst = dst;
  if (pad_dst != nullptr)
  {
    if (smoothed_shape[0] != pad_dst->width() || smoothed_shape[1] != pad_dst->height() ||
        smoothed_shape[2] != pad_dst->depth())
    {
      auto pad_size_x = smoothed_shape[0] - pad_dst->width();
      auto pad_size_y = smoothed_shape[1] - pad_dst->height();
      auto pad_size_z = smoothed_shape[2] - pad_dst->depth();
      pad_dst = tier1::pad_func(device, pad_dst, nullptr, pad_size_x, pad_size_y, pad_size_z, 0, true);
    }
  }
  else
  {
    pad_dst = Array::create(pad_input);
    pad_input->copyTo(pad_dst);
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
    tier1::unpad_func(device, pad_dst, dst, input_pad_size_x, input_pad_size_y, input_pad_size_z, true);
  }
  else
  {
    dst = pad_dst;
  }
  return dst;
}

} // namespace cle::tier8
