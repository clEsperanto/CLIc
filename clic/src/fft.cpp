
//
// This work is adapted from the clij2-fft repository <https://github.com/clij/clij2-fft/tree/master>
// from the work of Brian Northan <https://github.com/bnorthan> under the BSD-3-Clause License
//


#include "fft.hpp"
#include "cache.hpp"
#include "device.hpp"
#include "tier0.hpp"

#include <array>
#include <stdexcept>

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <tuple>
#include <vector>

#include "cle_fft.h"

namespace cle::fft
{

#if USE_CUDA

Array::Pointer
create_hermitian(const Array::Pointer & input)
{
  throw std::runtime_error("Error: FFT functions are not yet implemented for CUDA backend.");
}

auto
fft_pad_shape(const std::array<size_t, 3> & image_shape, const std::array<size_t, 3> & kernel_shape) -> std::array<size_t, 3>
{
  throw std::runtime_error("Error: FFT functions are not yet implemented for CUDA backend.");
}
auto
execOperationKernel(const Device::Pointer & device,
                    const std::string       name,
                    const Array::Pointer &  bufferA,
                    const Array::Pointer &  bufferB,
                    Array::Pointer          buffer_out,
                    const unsigned int      nElements) -> Array::Pointer
{
  throw std::runtime_error("Error: FFT functions are not yet implemented for CUDA backend.");
}
auto
execRemoveSmallValues(const Device::Pointer & device, Array::Pointer buffer, const unsigned int nElements) -> void
{
  throw std::runtime_error("Error: FFT functions are not yet implemented for CUDA backend.");
}
auto
execTotalVariationTerm(const Device::Pointer & device,
                       const Array::Pointer &  estimate,
                       const Array::Pointer &  correction,
                       Array::Pointer          variation,
                       float                   hx,
                       float                   hy,
                       float                   hz,
                       float                   regularization_factor) -> void
{
  throw std::runtime_error("Error: FFT functions are not yet implemented for CUDA backend.");
}
auto
performFFT(const Array::Pointer & input, Array::Pointer output) -> Array::Pointer
{
  throw std::runtime_error("Error: FFT functions are not yet implemented for CUDA backend.");
}
auto
performIFFT(const Array::Pointer & input, const Array::Pointer & output) -> void
{
  throw std::runtime_error("Error: FFT functions are not yet implemented for CUDA backend.");
}
auto
performConvolution(const Array::Pointer & input, const Array::Pointer & psf, const Array::Pointer & output, bool correlate) -> void
{
  throw std::runtime_error("Error: FFT functions are not yet implemented for CUDA backend.");
}
auto
performDeconvolution(const Array::Pointer & observe,
                     const Array::Pointer & psf,
                     Array::Pointer         normal,
                     Array::Pointer         estimate,
                     size_t                 iterations,
                     float                  regularization) -> Array::Pointer
{
  throw std::runtime_error("Error: FFT functions are not yet implemented for CUDA backend.");
}

#else // USE_CUDA ═════════════════════════════════════════════════════════════


auto
fft_pad_shape(const std::array<size_t, 3> & image_shape, const std::array<size_t, 3> & kernel_shape) -> std::array<size_t, 3>
{
  std::array<size_t, 3> new_shape;
  std::transform(image_shape.begin(), image_shape.end(), kernel_shape.begin(), new_shape.begin(), [](size_t img_dim, size_t ker_dim) {
    return img_dim + 2 * static_cast<size_t>(std::floor(ker_dim / 2.0));
  });
  return new_shape;
}


Array::Pointer
create_hermitian(const Array::Pointer & input)
{
  auto   ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(input->device());
  size_t hermitian_width = static_cast<size_t>(input->width() / 2) + 1;
  return Array::create(
    hermitian_width * 2, input->height(), input->depth(), input->dimension(), dType::COMPLEX, input->mtype(), input->device());
}

auto
configure(const Array::Pointer & array, VkFFTConfiguration & configuration) -> void
{
  configuration.numberBatches = 1;
  configuration.size[0] = array->width();
  configuration.size[1] = array->height();
  configuration.size[2] = array->depth();
  configuration.FFTdim = 1;
  if (configuration.size[1] > 1)
  {
    configuration.FFTdim++;
  }
  if (configuration.size[2] > 1)
  {
    configuration.FFTdim++;
  }

  configuration.normalize = 1;  // OFF, Normalization
  configuration.performR2C = 1; // ON, Real 2 Complex and reverse
  configuration.performDCT = 0; // OFF, Discrete Cosine Transform

  configuration.isInputFormatted = 1;
  configuration.inverseReturnToInputBuffer = 1;

  configuration.inputBufferStride[0] = configuration.size[0];
  configuration.inputBufferStride[1] = configuration.inputBufferStride[0] * configuration.size[1];
  configuration.inputBufferStride[2] = configuration.inputBufferStride[1] * configuration.size[2];
}

auto
get_cache_path(const Array::Pointer & output, const std::shared_ptr<OpenCLDevice> & ocl_device) -> std::filesystem::path
{
  std::ostringstream hashStream;
  hashStream << output->width() << "," << output->height() << "," << output->depth() << "," << output->dimension();
  const auto source_hash = DiskCache::hash(hashStream.str());
  const auto device_hash = DiskCache::hash(ocl_device->getInfo());
  return DiskCache::instance().getFilePath(device_hash, source_hash, "bin");
}

auto
load_kernel_cache(const std::filesystem::path & binary_path, VkFFTConfiguration & configuration) -> bool
{
  FILE * kernelCache = fopen(binary_path.string().c_str(), "rb");
  if (kernelCache == nullptr)
  {
    return false;
  }
  fseek(kernelCache, 0, SEEK_END);
  uint64_t str_len = ftell(kernelCache);
  fseek(kernelCache, 0, SEEK_SET);
  configuration.loadApplicationString = malloc(str_len);
  auto fsize = fread(configuration.loadApplicationString, str_len, 1, kernelCache);
  fclose(kernelCache);

  configuration.loadApplicationFromString = 1;
  configuration.saveApplicationToString = 0;
  return true;
}

auto
save_kernel_cache(const std::filesystem::path & binary_path, const VkFFTApplication & app) -> void
{
  std::filesystem::create_directories(binary_path.parent_path());
  FILE * kernelCache = fopen(binary_path.string().c_str(), "wb");
  fwrite(app.saveApplicationString, app.applicationStringSize, 1, kernelCache);
  fclose(kernelCache);
}

auto
performFFT(const Array::Pointer & input, Array::Pointer output) -> Array::Pointer
{
  // create hermitian buffer if output is not provided
  if (output == nullptr)
  {
    output = create_hermitian(input);
  }

  // fetch ocl device, context and queue
  auto ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(input->device());
  auto device = ocl_device->getCLDevice();
  auto context = ocl_device->getCLContext();
  auto queue = ocl_device->getCLCommandQueue();

  // configure VkFFT
  VkFFTConfiguration configuration{};
  configure(input, configuration);

  auto output_mem = static_cast<cl_mem>(output->get());
  auto input_mem = static_cast<cl_mem>(input->get());


  auto psize = static_cast<uint64_t>(output->bitsize());
  auto psizein = static_cast<uint64_t>(input->bitsize());
  configuration.bufferSize = &psize;
  configuration.inputBufferSize = &psizein;
  configuration.buffer = &output_mem;
  configuration.inputBuffer = &input_mem;
  configuration.outputBuffer = &output_mem;
  configuration.device = &device;
  configuration.context = &context;
  configuration.commandQueue = &queue;

  // manage jit-cache system
  const auto            use_cache = DiskCache::instance().isEnabled();
  std::filesystem::path binary_path;
  if (use_cache)
  {
    binary_path = get_cache_path(input, ocl_device);
    if (!load_kernel_cache(binary_path, configuration))
    {
      configuration.loadApplicationFromString = 0;
      configuration.saveApplicationToString = 1;
    }
  }

  // initialize VkFFT
  VkFFTApplication app = {};
  auto             resFFT = initializeVkFFT(&app, configuration);
  if (resFFT != VKFFT_SUCCESS)
  {
    throw std::runtime_error("Error: Failed to initialize VkFFT -> " + std::string(getVkFFTErrorString(resFFT)));
  }

  // free memory if needed, save to cache if needed
  if (use_cache && configuration.loadApplicationFromString)
  {
    free(configuration.loadApplicationString);
  }
  if (use_cache && configuration.saveApplicationToString)
  {
    save_kernel_cache(binary_path, app);
  }

  // execute VkFFT
  VkFFTLaunchParams launchParams = {};
  launchParams.commandQueue = &queue;
  resFFT = VkFFTAppend(&app, -1, &launchParams);
  if (resFFT != VKFFT_SUCCESS)
  {
    throw std::runtime_error("Error: Failed to execute VkFFT -> " + std::string(getVkFFTErrorString(resFFT)));
  }

  // wait for calculations to be finished before returning
  clFinish(queue);
  deleteVkFFT(&app);
  return output;
}


auto
performIFFT(const Array::Pointer & input, const Array::Pointer & output) -> void
{
  if (output == nullptr)
  {
    throw std::runtime_error("Error: ifft output buffer is null. Please provide an output buffer.");
  }

  // fetch ocl device, context and queue
  auto ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(input->device());
  auto device = ocl_device->getCLDevice();
  auto context = ocl_device->getCLContext();
  auto queue = ocl_device->getCLCommandQueue();

  // configure VkFFT
  VkFFTConfiguration configuration{};
  configure(output, configuration);

  auto input_mem = static_cast<cl_mem>(input->get());
  auto output_mem = static_cast<cl_mem>(output->get());

  auto input_size = static_cast<uint64_t>(input->bitsize());
  auto output_size = static_cast<uint64_t>(output->bitsize());
  configuration.bufferSize = &input_size;
  configuration.inputBufferSize = &output_size;
  configuration.buffer = &input_mem;
  configuration.inputBuffer = &output_mem;
  configuration.outputBuffer = &input_mem;
  configuration.device = &device;
  configuration.context = &context;
  configuration.commandQueue = &queue;

  // manage jit-cache system
  const auto            use_cache = DiskCache::instance().isEnabled();
  std::filesystem::path binary_path;
  if (use_cache)
  {
    binary_path = get_cache_path(output, ocl_device);
    if (!load_kernel_cache(binary_path, configuration))
    {
      configuration.loadApplicationFromString = 0;
      configuration.saveApplicationToString = 1;
    }
  }

  // initialize VkFFT
  VkFFTApplication app = {};
  auto             resFFT = initializeVkFFT(&app, configuration);
  if (resFFT != VKFFT_SUCCESS)
  {
    throw std::runtime_error("Error: Failed to initialize VkFFT -> " + std::string(getVkFFTErrorString(resFFT)));
  }

  // free memory if needed, save to cache if needed
  if (use_cache && configuration.loadApplicationFromString)
  {
    free(configuration.loadApplicationString);
  }
  if (use_cache && configuration.saveApplicationToString)
  {
    save_kernel_cache(binary_path, app);
  }

  // execute VkFFT
  VkFFTLaunchParams launchParams = {};
  launchParams.commandQueue = &queue;
  resFFT = VkFFTAppend(&app, 1, &launchParams);
  if (resFFT != VKFFT_SUCCESS)
  {
    throw std::runtime_error("Error: Failed to execute VkFFT -> " + std::string(getVkFFTErrorString(resFFT)));
  }

  // wait for calculations to be finished before returning
  clFinish(queue);
  deleteVkFFT(&app);
}


auto
performConvolution(const Array::Pointer & input, const Array::Pointer & psf, const Array::Pointer & output, bool correlate) -> void
{
  auto device = input->device();

  // forward fft of input and psf
  auto fft_input = performFFT(input, nullptr);
  auto fft_psf = performFFT(psf, nullptr);
  auto fft_out = Array::create(fft_psf);

  // complex multiply input and psf
  std::string kernel_name = correlate ? "vecComplexConjugateMultiply" : "vecComplexMultiply";
  execOperationKernel(device, kernel_name, fft_input, fft_psf, fft_out, fft_input->size() / 2);

  // Inverse to get convolved
  performIFFT(fft_out, output);
}


auto
performDeconvolution(const Array::Pointer & observe,
                     const Array::Pointer & psf,
                     Array::Pointer         normal,
                     Array::Pointer         estimate,
                     size_t                 iterations,
                     float                  regularization) -> Array::Pointer
{
  // fetch ocl device, context and queue
  auto device = observe->device();

  // check if total variation is used
  bool use_tv = regularization > 0;

  // create temp and output real buffers
  auto reblurred = Array::create(observe);
  if (estimate == nullptr)
  {
    estimate = Array::create(observe);
    observe->copyTo(estimate);
  }
  // create fft buffers
  auto fft_psf = create_hermitian(psf);
  auto fft_estimate = create_hermitian(estimate);

  // create variation buffer, if used
  Array::Pointer variation = nullptr;
  if (use_tv)
  {
    variation = Array::create(observe);
  }

  // clean normal buffer, if used
  if (normal != nullptr)
  {
    // remove small values from normal, if used
    execRemoveSmallValues(device, normal, normal->size());
  }

  // FFT of PSF
  performFFT(psf, fft_psf);

  // Richardson Lucy - deconvolution loop
  for (size_t i = 0; i < iterations; i++)
  {
    // FFT of estimate
    performFFT(estimate, fft_estimate);

    // complex multiply estimate FFT and PSF FFT
    execOperationKernel(device, "vecComplexMultiply", fft_estimate, fft_psf, fft_estimate, fft_estimate->size() / 2);

    // Inverse FFT of estimate to get reblurred
    performIFFT(fft_estimate, reblurred);

    // Divide observed by reblurred
    execOperationKernel(device, "vecDiv", observe, reblurred, reblurred, observe->size());

    // FFT of reblurred into estimate
    performFFT(reblurred, fft_estimate);

    // Correlate above result with PSF
    execOperationKernel(device, "vecComplexConjugateMultiply", fft_estimate, fft_psf, fft_estimate, fft_estimate->size() / 2);

    // Inverse FFT of estimate to get reblurred
    performIFFT(fft_estimate, reblurred);

    // total variation multiply by variation factor, if used
    if (use_tv)
    {
      // calculate total variation
      execTotalVariationTerm(device, estimate, reblurred, variation, 1.0, 1.0, 3.0, regularization);
      // multiply by variation factor
      execOperationKernel(device, "vecMul", estimate, variation, estimate, estimate->size());
    }
    else
    {
      // multiply estimate by update factor
      execOperationKernel(device, "vecMul", estimate, reblurred, estimate, estimate->size());
    }

    if (normal != nullptr)
    {
      // divide estimate by normal, if used
      execOperationKernel(device, "vecDiv", estimate, normal, estimate, estimate->size());
    }

    // wait for calculations to be finished before next iteration
    device->finish();
  }

  return estimate;
}


auto
execOperationKernel(const Device::Pointer & device,
                    const std::string       name,
                    const Array::Pointer &  inputA,
                    const Array::Pointer &  inputB,
                    Array::Pointer          output,
                    const unsigned int      nElements) -> Array::Pointer
{
  const size_t LOCAL_ITEM_SIZE = std::min(256, static_cast<int>(device->getMaximumWorkGroupSize()));
  size_t       globalItemSize =
    static_cast<size_t>(ceil(static_cast<double>(nElements) / static_cast<double>(LOCAL_ITEM_SIZE)) * LOCAL_ITEM_SIZE);

  const RangeArray    global_range = { globalItemSize, 1, 1 };
  const RangeArray    local_range = { LOCAL_ITEM_SIZE, 1, 1 };
  const KernelInfo    kernel = { name, kernel::fft };
  const ParameterList params = { { "a", inputA }, { "b", inputB }, { "c", output }, { "n", nElements } };
  native_execute(device, kernel, params, global_range, local_range);

  return output;
}

auto
execRemoveSmallValues(const Device::Pointer & device, Array::Pointer buffer, const unsigned int nElements) -> void
{
  const size_t LOCAL_ITEM_SIZE = std::min(256, static_cast<int>(device->getMaximumWorkGroupSize()));
  size_t       globalItemSize =
    static_cast<size_t>(ceil(static_cast<double>(nElements) / static_cast<double>(LOCAL_ITEM_SIZE)) * LOCAL_ITEM_SIZE);
  const RangeArray    global_range = { globalItemSize, 1, 1 };
  const RangeArray    local_range = { LOCAL_ITEM_SIZE, 1, 1 };
  KernelInfo          kernel = { "removeSmallValues", kernel::fft };
  const ParameterList params = { { "a", buffer }, { "n", nElements } };
  native_execute(device, kernel, params, global_range, local_range);
}

auto
execTotalVariationTerm(const Device::Pointer & device,
                       const Array::Pointer &  estimate,
                       const Array::Pointer &  correction,
                       Array::Pointer          variation,
                       float                   hx,
                       float                   hy,
                       float                   hz,
                       float                   regularization_factor) -> void
{

  unsigned int nx = estimate->width();
  unsigned int ny = estimate->height();
  unsigned int nz = estimate->depth();

  /// use normal global range and set local to default to avoid local size optimization
  // const RangeArray local_range = determine_local_size(device->getMaximumWorkGroupSize(), estimate->dim());
  // const RangeArray    global_range = {
  //   (static_cast<size_t>(nx) + local_range[0] - 1) / local_range[0] * local_range[0],
  //   (static_cast<size_t>(ny) + local_range[1] - 1) / local_range[1] * local_range[1],
  //   (static_cast<size_t>(nz) + local_range[2] - 1) / local_range[2] * local_range[2]
  // };
  const RangeArray local_range = { 0, 0, 0 };
  const RangeArray global_range = { static_cast<size_t>(nx), static_cast<size_t>(ny), static_cast<size_t>(nz) };

  KernelInfo          kernel = { "totalVariationTerm", kernel::fft };
  const ParameterList params = { { "estimate", estimate },
                                 { "correction", correction },
                                 { "variation", variation },
                                 { "Nx", nx },
                                 { "Ny", ny },
                                 { "Nz", nz },
                                 { "hx", hx },
                                 { "hy", hy },
                                 { "hz", hz },
                                 { "regularizationFactor", regularization_factor } };
  native_execute(device, kernel, params, global_range, local_range);
}

#endif // USE_CUDA ═════════════════════════════════════════════════════════════

} // namespace cle::fft
