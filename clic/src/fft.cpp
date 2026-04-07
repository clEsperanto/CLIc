
//
// This work is adapted from the clij2-fft repository <https://github.com/clij/clij2-fft/tree/master>
// from the work of Brian Northan <https://github.com/bnorthan> under the BSD-3-Clause License
//


#include "fft.hpp"
#include "cache.hpp"
#include "device.hpp"
#include "tier0.hpp"
#include "vkFFT.h"

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

#if USE_METAL
static auto
metalExecuteAppend(VkFFTApplication * app, int direction, void * metalQueue) -> VkFFTResult
{
  auto * queue = static_cast<MTL::CommandQueue *>(metalQueue);
  auto * cmdBuf = queue->commandBuffer();
  auto * encoder = cmdBuf->computeCommandEncoder();

  VkFFTLaunchParams lp = {};
  lp.commandBuffer = cmdBuf;
  lp.commandEncoder = encoder;

  auto res = VkFFTAppend(app, direction, &lp);

  encoder->endEncoding();
  cmdBuf->commit();
  cmdBuf->waitUntilCompleted();

  return res;
}
#endif

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
get_cache_path(const Array::Pointer & output, const Device::Pointer & device) -> std::filesystem::path
{
  std::ostringstream hashStream;
  hashStream << output->width() << "," << output->height() << "," << output->depth() << "," << output->dimension();
  const auto source_hash = DiskCache::hash(hashStream.str());
  const auto device_hash = DiskCache::hash(device->getInfo());
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

  // configure VkFFT
  VkFFTConfiguration configuration{};
  configure(input, configuration);


#if USE_CUDA
  auto cu_device = std::dynamic_pointer_cast<CUDADevice>(input->device());
  auto device = cu_device->getCUDADevice();
  auto stream = cu_device->getCUDAStream();

  void * output_mem = output->get();
  void * input_mem = input->get();

  auto output_size = static_cast<uint64_t>(output->bitsize());
  auto input_size = static_cast<uint64_t>(input->bitsize());
  configuration.bufferSize = &output_size;
  configuration.inputBufferSize = &input_size;
  configuration.buffer = &output_mem;
  configuration.inputBuffer = &input_mem;
  configuration.outputBuffer = &output_mem;
  configuration.device = &device;
  configuration.stream = &stream;
  configuration.num_streams = 1;
#elif USE_OPENCL
  // fetch ocl device, context and queue
  auto ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(input->device());
  auto device = ocl_device->getCLDevice();
  auto context = ocl_device->getCLContext();
  auto queue = ocl_device->getCLCommandQueue();

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
#elif USE_METAL
  auto metal_device = std::dynamic_pointer_cast<MetalDevice>(input->device());

  void * output_mem = output->get();
  void * input_mem = input->get();

  auto output_size = static_cast<uint64_t>(output->bitsize());
  auto input_size = static_cast<uint64_t>(input->bitsize());
  configuration.device = static_cast<MTL::Device *>(metal_device->getMetalDevice());
  configuration.queue = static_cast<MTL::CommandQueue *>(metal_device->getMetalCommandQueue());
  configuration.buffer = reinterpret_cast<MTL::Buffer **>(&output_mem);
  configuration.inputBuffer = reinterpret_cast<MTL::Buffer **>(&input_mem);
  configuration.outputBuffer = reinterpret_cast<MTL::Buffer **>(&output_mem);
  configuration.bufferSize = &output_size;
  configuration.inputBufferSize = &input_size;
#endif

  // manage jit-cache system (disabled for Metal backend)
#if !USE_METAL
  const auto            use_cache = DiskCache::instance().isEnabled();
  std::filesystem::path binary_path;
  if (use_cache)
  {
    binary_path = get_cache_path(input, input->device());
    if (!load_kernel_cache(binary_path, configuration))
    {
      configuration.loadApplicationFromString = 0;
      configuration.saveApplicationToString = 1;
    }
  }
#endif

  // initialize VkFFT
  VkFFTApplication app = {};
  auto             resFFT = initializeVkFFT(&app, configuration);
  if (resFFT != VKFFT_SUCCESS)
  {
    throw std::runtime_error("Error: Failed to initialize VkFFT -> " + std::string(getVkFFTErrorString(resFFT)));
  }

#if !USE_METAL
  // free memory if needed, save to cache if needed
  if (use_cache && configuration.loadApplicationFromString)
  {
    free(configuration.loadApplicationString);
  }
  if (use_cache && configuration.saveApplicationToString)
  {
    save_kernel_cache(binary_path, app);
  }
#endif

  // execute VkFFT
#if USE_METAL
  resFFT = metalExecuteAppend(&app, -1, metal_device->getMetalCommandQueue());
#else
  VkFFTLaunchParams launchParams = {};
#  if USE_OPENCL
  launchParams.commandQueue = &queue;
#  endif
  resFFT = VkFFTAppend(&app, -1, &launchParams);
#endif
  if (resFFT != VKFFT_SUCCESS)
  {
    throw std::runtime_error("Error: Failed to execute VkFFT -> " + std::string(getVkFFTErrorString(resFFT)));
  }

  // wait for calculations to be finished before returning
  input->device()->finish();
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

  // configure VkFFT
  VkFFTConfiguration configuration{};
  configure(output, configuration);

#if USE_CUDA
  auto cu_device = std::dynamic_pointer_cast<CUDADevice>(input->device());
  auto device = cu_device->getCUDADevice();
  auto stream = cu_device->getCUDAStream();

  void * input_mem = input->get();
  void * output_mem = output->get();

  auto input_size = static_cast<uint64_t>(input->bitsize());
  auto output_size = static_cast<uint64_t>(output->bitsize());
  configuration.bufferSize = &input_size;
  configuration.inputBufferSize = &output_size;
  configuration.buffer = &input_mem;
  configuration.inputBuffer = &output_mem;
  configuration.outputBuffer = &input_mem;
  configuration.device = &device;
  configuration.stream = &stream;
  configuration.num_streams = 1;
#elif USE_OPENCL
  // fetch ocl device, context and queue
  auto ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(input->device());
  auto device = ocl_device->getCLDevice();
  auto context = ocl_device->getCLContext();
  auto queue = ocl_device->getCLCommandQueue();

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
#elif USE_METAL
  auto metal_device = std::dynamic_pointer_cast<MetalDevice>(input->device());

  void * input_mem = input->get();
  void * output_mem = output->get();

  auto input_size = static_cast<uint64_t>(input->bitsize());
  auto output_size = static_cast<uint64_t>(output->bitsize());
  configuration.device = static_cast<MTL::Device *>(metal_device->getMetalDevice());
  configuration.queue = static_cast<MTL::CommandQueue *>(metal_device->getMetalCommandQueue());
  configuration.buffer = reinterpret_cast<MTL::Buffer **>(&input_mem);
  configuration.inputBuffer = reinterpret_cast<MTL::Buffer **>(&output_mem);
  configuration.outputBuffer = reinterpret_cast<MTL::Buffer **>(&input_mem);
  configuration.bufferSize = &input_size;
  configuration.inputBufferSize = &output_size;
#endif

  // manage jit-cache system (disabled for Metal backend)
#if !USE_METAL
  const auto            use_cache = DiskCache::instance().isEnabled();
  std::filesystem::path binary_path;
  if (use_cache)
  {
    binary_path = get_cache_path(output, input->device());
    if (!load_kernel_cache(binary_path, configuration))
    {
      configuration.loadApplicationFromString = 0;
      configuration.saveApplicationToString = 1;
    }
  }
#endif

  // initialize VkFFT
  VkFFTApplication app = {};
  auto             resFFT = initializeVkFFT(&app, configuration);
  if (resFFT != VKFFT_SUCCESS)
  {
    throw std::runtime_error("Error: Failed to initialize VkFFT -> " + std::string(getVkFFTErrorString(resFFT)));
  }

#if !USE_METAL
  // free memory if needed, save to cache if needed
  if (use_cache && configuration.loadApplicationFromString)
  {
    free(configuration.loadApplicationString);
  }
  if (use_cache && configuration.saveApplicationToString)
  {
    save_kernel_cache(binary_path, app);
  }
#endif

  // execute VkFFT
#if USE_METAL
  resFFT = metalExecuteAppend(&app, 1, metal_device->getMetalCommandQueue());
#else
  VkFFTLaunchParams launchParams = {};
#  if USE_OPENCL
  launchParams.commandQueue = &queue;
#  endif
  resFFT = VkFFTAppend(&app, 1, &launchParams);
#endif
  if (resFFT != VKFFT_SUCCESS)
  {
    throw std::runtime_error("Error: Failed to execute VkFFT -> " + std::string(getVkFFTErrorString(resFFT)));
  }

  // wait for calculations to be finished before returning
  input->device()->finish();
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
  auto device = observe->device();
  bool use_tv = regularization > 0;

  auto reblurred = Array::create(observe);
  if (estimate == nullptr)
  {
    estimate = Array::create(observe);
    observe->copyTo(estimate);
  }
  auto fft_psf = create_hermitian(psf);
  auto fft_estimate = create_hermitian(estimate);

  Array::Pointer variation = nullptr;
  if (use_tv)
    variation = Array::create(observe);

  if (normal != nullptr)
    execRemoveSmallValues(device, normal, normal->size());

  // FFT of PSF (single init+exec+delete – done only once)
  performFFT(psf, fft_psf);

  // ── Pre-initialise VkFFT apps once for the iterative loop ─────────────────
  // Calling performFFT/performIFFT inside the loop would trigger one
  // initializeVkFFT (= NVRTC/JIT compilation) per call, making CUDA ~100x
  // slower than OpenCL for many iterations.  Instead we build two apps here
  // and call VkFFTAppend directly in the loop.
  //
  // Forward FFT config stores a pointer to `fft_real_in_mem`.  We update that
  // variable before each VkFFTAppend to switch between estimate and reblurred
  // as the real-space input without reinitialising the app.

  auto real_size = static_cast<uint64_t>(estimate->bitsize());
  auto complex_size = static_cast<uint64_t>(fft_estimate->bitsize());

  VkFFTConfiguration fft_cfg{};
  configure(estimate, fft_cfg);

  VkFFTConfiguration ifft_cfg{};
  configure(reblurred, ifft_cfg);
#if USE_CUDA
  auto cu_dev = std::dynamic_pointer_cast<CUDADevice>(device);
  auto cuda_device = cu_dev->getCUDADevice();
  auto cuda_stream = cu_dev->getCUDAStream();

  void * fft_real_in_mem = estimate->get(); // mutable: updated per VkFFTAppend
  void * fft_complex_mem = fft_estimate->get();
  void * ifft_real_out = reblurred->get();

  fft_cfg.bufferSize = &complex_size;
  fft_cfg.inputBufferSize = &real_size;
  fft_cfg.buffer = &fft_complex_mem;
  fft_cfg.inputBuffer = &fft_real_in_mem; // pointer to mutable local
  fft_cfg.outputBuffer = &fft_complex_mem;
  fft_cfg.device = &cuda_device;
  fft_cfg.stream = &cuda_stream;
  fft_cfg.num_streams = 1;

  ifft_cfg.bufferSize = &complex_size;
  ifft_cfg.inputBufferSize = &real_size;
  ifft_cfg.buffer = &fft_complex_mem;
  ifft_cfg.inputBuffer = &ifft_real_out;
  ifft_cfg.outputBuffer = &fft_complex_mem;
  ifft_cfg.device = &cuda_device;
  ifft_cfg.stream = &cuda_stream;
  ifft_cfg.num_streams = 1;
#elif USE_OPENCL
  auto ocl_dev = std::dynamic_pointer_cast<OpenCLDevice>(device);
  auto ocl_device = ocl_dev->getCLDevice();
  auto ocl_context = ocl_dev->getCLContext();
  auto ocl_queue = ocl_dev->getCLCommandQueue();

  cl_mem fft_real_in_mem = static_cast<cl_mem>(estimate->get()); // mutable
  cl_mem fft_complex_mem = static_cast<cl_mem>(fft_estimate->get());
  cl_mem ifft_real_out = static_cast<cl_mem>(reblurred->get());

  fft_cfg.bufferSize = &complex_size;
  fft_cfg.inputBufferSize = &real_size;
  fft_cfg.buffer = &fft_complex_mem;
  fft_cfg.inputBuffer = &fft_real_in_mem; // pointer to mutable local
  fft_cfg.outputBuffer = &fft_complex_mem;
  fft_cfg.device = &ocl_device;
  fft_cfg.context = &ocl_context;
  fft_cfg.commandQueue = &ocl_queue;

  ifft_cfg.bufferSize = &complex_size;
  ifft_cfg.inputBufferSize = &real_size;
  ifft_cfg.buffer = &fft_complex_mem;
  ifft_cfg.inputBuffer = &ifft_real_out;
  ifft_cfg.outputBuffer = &fft_complex_mem;
  ifft_cfg.device = &ocl_device;
  ifft_cfg.context = &ocl_context;
  ifft_cfg.commandQueue = &ocl_queue;
#elif USE_METAL
  auto metal_dev = std::dynamic_pointer_cast<MetalDevice>(device);

  void * fft_real_in_mem = estimate->get(); // mutable: updated per VkFFTAppend
  void * fft_complex_mem = fft_estimate->get();
  void * ifft_real_out = reblurred->get();

  fft_cfg.device = static_cast<MTL::Device *>(metal_dev->getMetalDevice());
  fft_cfg.queue = static_cast<MTL::CommandQueue *>(metal_dev->getMetalCommandQueue());
  fft_cfg.bufferSize = &complex_size;
  fft_cfg.inputBufferSize = &real_size;
  fft_cfg.buffer = reinterpret_cast<MTL::Buffer **>(&fft_complex_mem);
  fft_cfg.inputBuffer = reinterpret_cast<MTL::Buffer **>(&fft_real_in_mem);
  fft_cfg.outputBuffer = reinterpret_cast<MTL::Buffer **>(&fft_complex_mem);

  ifft_cfg.device = static_cast<MTL::Device *>(metal_dev->getMetalDevice());
  ifft_cfg.queue = static_cast<MTL::CommandQueue *>(metal_dev->getMetalCommandQueue());
  ifft_cfg.bufferSize = &complex_size;
  ifft_cfg.inputBufferSize = &real_size;
  ifft_cfg.buffer = reinterpret_cast<MTL::Buffer **>(&fft_complex_mem);
  ifft_cfg.inputBuffer = reinterpret_cast<MTL::Buffer **>(&ifft_real_out);
  ifft_cfg.outputBuffer = reinterpret_cast<MTL::Buffer **>(&fft_complex_mem);
#endif

  VkFFTApplication fft_app = {};
  VkFFTApplication ifft_app = {};
  auto             res = initializeVkFFT(&fft_app, fft_cfg);
  if (res != VKFFT_SUCCESS)
    throw std::runtime_error("Error: Failed to initialise forward VkFFT -> " + std::string(getVkFFTErrorString(res)));
  res = initializeVkFFT(&ifft_app, ifft_cfg);
  if (res != VKFFT_SUCCESS)
  {
    deleteVkFFT(&fft_app);
    throw std::runtime_error("Error: Failed to initialise inverse VkFFT -> " + std::string(getVkFFTErrorString(res)));
  }

  // ── Richardson-Lucy deconvolution loop ────────────────────────────────────
#if !USE_METAL
  VkFFTLaunchParams lp = {};
#  if USE_OPENCL
  lp.commandQueue = &ocl_queue;
#  endif
#endif

  for (size_t i = 0; i < iterations; i++)
  {
    // Forward FFT of estimate → fft_estimate
#if USE_CUDA
    fft_real_in_mem = estimate->get();
#elif USE_OPENCL
    fft_real_in_mem = static_cast<cl_mem>(estimate->get());
#elif USE_METAL
    fft_real_in_mem = estimate->get();
#endif
#if USE_METAL
    res = metalExecuteAppend(&fft_app, -1, metal_dev->getMetalCommandQueue());
#else
    res = VkFFTAppend(&fft_app, -1, &lp);
#endif
    if (res != VKFFT_SUCCESS)
      throw std::runtime_error("Error: VkFFTAppend (forward) -> " + std::string(getVkFFTErrorString(res)));

    // complex multiply: fft_estimate × fft_psf → fft_estimate
    execOperationKernel(device, "vecComplexMultiply", fft_estimate, fft_psf, fft_estimate, fft_estimate->size() / 2);

    // Inverse FFT: fft_estimate → reblurred
#if USE_METAL
    res = metalExecuteAppend(&ifft_app, 1, metal_dev->getMetalCommandQueue());
#else
    res = VkFFTAppend(&ifft_app, 1, &lp);
#endif
    if (res != VKFFT_SUCCESS)
      throw std::runtime_error("Error: VkFFTAppend (inverse) -> " + std::string(getVkFFTErrorString(res)));

    // Divide observed by reblurred
    execOperationKernel(device, "vecDiv", observe, reblurred, reblurred, observe->size());

    // Forward FFT of reblurred → fft_estimate
#if USE_CUDA
    fft_real_in_mem = reblurred->get();
#elif USE_OPENCL
    fft_real_in_mem = static_cast<cl_mem>(reblurred->get());
#elif USE_METAL
    fft_real_in_mem = reblurred->get();
#endif
#if USE_METAL
    res = metalExecuteAppend(&fft_app, -1, metal_dev->getMetalCommandQueue());
#else
    res = VkFFTAppend(&fft_app, -1, &lp);
#endif
    if (res != VKFFT_SUCCESS)
      throw std::runtime_error("Error: VkFFTAppend (forward) -> " + std::string(getVkFFTErrorString(res)));

    // Correlate: complex conjugate multiply of fft_estimate with fft_psf
    execOperationKernel(device, "vecComplexConjugateMultiply", fft_estimate, fft_psf, fft_estimate, fft_estimate->size() / 2);

    // Inverse FFT: fft_estimate → reblurred
#if USE_METAL
    res = metalExecuteAppend(&ifft_app, 1, metal_dev->getMetalCommandQueue());
#else
    res = VkFFTAppend(&ifft_app, 1, &lp);
#endif
    if (res != VKFFT_SUCCESS)
      throw std::runtime_error("Error: VkFFTAppend (inverse) -> " + std::string(getVkFFTErrorString(res)));

    if (use_tv)
    {
      execTotalVariationTerm(device, estimate, reblurred, variation, 1.0, 1.0, 3.0, regularization);
      execOperationKernel(device, "vecMul", estimate, variation, estimate, estimate->size());
    }
    else
    {
      execOperationKernel(device, "vecMul", estimate, reblurred, estimate, estimate->size());
    }

    if (normal != nullptr)
      execOperationKernel(device, "vecDiv", estimate, normal, estimate, estimate->size());

    device->finish();
  }

  deleteVkFFT(&fft_app);
  deleteVkFFT(&ifft_app);
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

} // namespace cle::fft
