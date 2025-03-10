
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

#include "cle_fft.h"

namespace cle::fft
{

// auto
// SetupFFT() -> cl_int
// {
//   clfftSetupData fftSetup;
//   cl_int         err = clfftInitSetupData(&fftSetup);
//   err = clfftSetup(&fftSetup);
//   if (err != CL_SUCCESS)
//   {
//     throw std::runtime_error("Failed to setup clFFT");
//   }
//   return err;
// }


Array::Pointer
create_hermitian(const Array::Pointer & real_buf)
{
  auto   ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(real_buf->device());
  size_t hermitian_width = static_cast<size_t>(real_buf->width() / 2) + 1;
  return Array::create(hermitian_width * 2,
                       real_buf->height(),
                       real_buf->depth(),
                       real_buf->dimension(),
                       real_buf->dtype(),
                       real_buf->mtype(),
                       real_buf->device());
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

  // configuration.saveApplicationToString = 1;
  // configuration.loadApplicationFromString = 1;
}


auto
performFFT(const Array::Pointer & input, Array::Pointer output) -> Array::Pointer
{
  if (output == nullptr)
  {
    output = create_hermitian(input);
  }

  auto ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(input->device());
  auto device = ocl_device->getCLDevice();
  auto context = ocl_device->getCLContext();
  auto queue = ocl_device->getCLCommandQueue();

  VkFFTConfiguration configuration{};
  configure(input, configuration);

  auto psize = static_cast<uint64_t>(output->bitsize());
  auto psizein = static_cast<uint64_t>(input->bitsize());
  configuration.bufferSize = &psize;
  configuration.inputBufferSize = &psizein;
  configuration.buffer = static_cast<cl_mem *>(*output->get());
  configuration.inputBuffer = static_cast<cl_mem *>(*input->get());
  configuration.outputBuffer = static_cast<cl_mem *>(*output->get());
  configuration.device = &device;
  configuration.context = &context;
  configuration.commandQueue = &queue;

  const auto            use_cache = is_cache_enabled();
  std::filesystem::path binary_path;
  if (use_cache)
  {
    std::ostringstream hashStream;
    hashStream << output->width() << "," << output->height() << "," << output->depth() << "," << output->dimension();
    std::hash<std::string> hasher;
    const auto             source_hash = std::to_string(hasher(hashStream.str()));
    const auto             device_hash = std::to_string(hasher(ocl_device->getInfo()));

    binary_path = CACHE_FOLDER_PATH / std::filesystem::path(device_hash) / std::filesystem::path(source_hash + ".bin");

    FILE *   kernelCache;
    uint64_t str_len;
    kernelCache = fopen(binary_path.c_str(), "rb");
    if (kernelCache == nullptr)
    {
      configuration.loadApplicationFromString = 0;
      configuration.saveApplicationToString = 1;
    }
    else
    {
      configuration.loadApplicationFromString = 1;
      configuration.saveApplicationToString = 0;
      fseek(kernelCache, 0, SEEK_END);
      str_len = ftell(kernelCache);
      fseek(kernelCache, 0, SEEK_SET);
      configuration.loadApplicationString = malloc(str_len);
      fread(configuration.loadApplicationString, str_len, 1, kernelCache);
      fclose(kernelCache);
    }
  }

  VkFFTApplication app = {};
  auto             resFFT = initializeVkFFT(&app, configuration);
  if (resFFT != VKFFT_SUCCESS)
  {
    throw std::runtime_error("Error: Failed to initialize VkFFT -> " + std::string(getVkFFTErrorString(resFFT)));
  }

  if (use_cache && configuration.loadApplicationFromString)
  {
    free(configuration.loadApplicationString);
  }

  if (use_cache && configuration.saveApplicationToString)
  {
    FILE * kernelCache;
    kernelCache = fopen(binary_path.c_str(), "wb");
    fwrite(app.saveApplicationString, app.applicationStringSize, 1, kernelCache);
    fclose(kernelCache);
  }

  VkFFTLaunchParams launchParams = {};
  launchParams.commandQueue = &queue;

  resFFT = VkFFTAppend(&app, -1, &launchParams);
  if (resFFT != VKFFT_SUCCESS)
  {
    throw std::runtime_error("Error: Failed to execute VkFFT -> " + std::string(getVkFFTErrorString(resFFT)));
  }
  clFinish(queue);

  deleteVkFFT(&app);
  return output;
}


auto
performIFFT(const Array::Pointer & input, Array::Pointer output) -> void
{
  auto ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(input->device());
  auto device = ocl_device->getCLDevice();
  auto context = ocl_device->getCLContext();
  auto queue = ocl_device->getCLCommandQueue();

  VkFFTConfiguration configuration{};
  configure(output, configuration);

  auto psize = static_cast<uint64_t>(input->bitsize());
  auto psizein = static_cast<uint64_t>(output->bitsize());
  configuration.bufferSize = &psize;
  configuration.inputBufferSize = &psizein;
  configuration.buffer = static_cast<cl_mem *>(*input->get());
  configuration.inputBuffer = static_cast<cl_mem *>(*output->get());
  configuration.outputBuffer = static_cast<cl_mem *>(*input->get());
  configuration.device = &device;
  configuration.context = &context;
  configuration.commandQueue = &queue;

  const auto            use_cache = is_cache_enabled();
  std::filesystem::path binary_path;
  if (use_cache)
  {
    std::ostringstream hashStream;
    hashStream << output->width() << "," << output->height() << "," << output->depth() << "," << output->dimension();
    std::hash<std::string> hasher;
    const auto             source_hash = std::to_string(hasher(hashStream.str()));
    const auto             device_hash = std::to_string(hasher(ocl_device->getInfo()));

    binary_path = CACHE_FOLDER_PATH / std::filesystem::path(device_hash) / std::filesystem::path(source_hash + ".bin");

    FILE *   kernelCache;
    uint64_t str_len;
    kernelCache = fopen(binary_path.c_str(), "rb");
    if (kernelCache == nullptr)
    {
      configuration.loadApplicationFromString = 0;
      configuration.saveApplicationToString = 1;
    }
    else
    {
      configuration.loadApplicationFromString = 1;
      configuration.saveApplicationToString = 0;
      fseek(kernelCache, 0, SEEK_END);
      str_len = ftell(kernelCache);
      fseek(kernelCache, 0, SEEK_SET);
      configuration.loadApplicationString = malloc(str_len);
      fread(configuration.loadApplicationString, str_len, 1, kernelCache);
      fclose(kernelCache);
    }
  }

  VkFFTApplication app = {};
  auto             resFFT = initializeVkFFT(&app, configuration);
  if (resFFT != VKFFT_SUCCESS)
  {
    throw std::runtime_error("Error: Failed to initialize VkFFT -> " + std::string(getVkFFTErrorString(resFFT)));
  }

  if (use_cache && configuration.loadApplicationFromString)
  {
    free(configuration.loadApplicationString);
  }

  if (use_cache && configuration.saveApplicationToString)
  {
    FILE * kernelCache;
    kernelCache = fopen(binary_path.c_str(), "wb");
    fwrite(app.saveApplicationString, app.applicationStringSize, 1, kernelCache);
    fclose(kernelCache);
  }

  VkFFTLaunchParams launchParams = {};
  launchParams.commandQueue = &queue;

  resFFT = VkFFTAppend(&app, 1, &launchParams);
  if (resFFT != VKFFT_SUCCESS)
  {
    throw std::runtime_error("Error: Failed to execute VkFFT -> " + std::string(getVkFFTErrorString(resFFT)));
  }
  clFinish(queue);

  deleteVkFFT(&app);
}


auto
performConvolution(const Array::Pointer & input, const Array::Pointer & psf, Array::Pointer output, bool correlate)
  -> Array::Pointer
{
  auto device = input->device();
  if (output == nullptr)
  {
    output = Array::create(input);
  }

  // forward fft of input and psf
  auto fft_input = performFFT(input, nullptr);
  auto fft_psf = performFFT(psf, nullptr);
  auto fft_out = Array::create(fft_psf);

  // complex multiply input and psf
  std::string kernel_name = correlate ? "vecComplexConjugateMultiply" : "vecComplexMultiply";
  execOperationKernel(device, kernel_name, fft_input, fft_psf, fft_out, fft_input->size() / 2);

  // Inverse to get convolved
  performIFFT(fft_out, output);

  return output;
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
    execOperationKernel(
      device, "vecComplexConjugateMultiply", fft_estimate, fft_psf, fft_estimate, fft_estimate->size() / 2);

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

    // // wait for calculations to be finished before next iteration
    device->finish();
  }

  return estimate;
}


// auto
// bake_forward(const Array::Pointer & real) -> clfftPlanHandle
// {
//   auto ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(real->device());
//   auto ctx = ocl_device->getCLContext();
//   auto queue = ocl_device->getCLCommandQueue();


//   /* FFT library related declarations */
//   clfftDim            dim;
//   std::vector<size_t> clLengths;
//   std::vector<size_t> inStride;
//   std::vector<size_t> outStride;
//   size_t              hermitian_width = static_cast<size_t>(real->width() / 2) + 1;
//   // Reserve space for the vectors
//   clLengths.reserve(3);
//   inStride.reserve(3);
//   outStride.reserve(3);
//   switch (real->dimension())
//   {
//     case 1:
//       dim = CLFFT_1D;
//       clLengths = { real->width() };
//       inStride = { 1 };
//       outStride = { 1 };
//       break;
//     case 2:
//       dim = CLFFT_2D;
//       clLengths = { real->width(), real->height() };
//       inStride = { 1, real->width() };
//       outStride = { 1, hermitian_width };
//       break;
//     case 3:
//       dim = CLFFT_3D;
//       clLengths = { real->width(), real->height(), real->depth() };
//       inStride = { 1, real->width(), real->width() * real->height() };
//       outStride = { 1, hermitian_width, hermitian_width * real->height() };
//       break;
//     default:
//       throw std::runtime_error("Invalid FFT dimension");
//   }

//   /* Create a default plan for a complex FFT. */
//   clfftPlanHandle planHandle;
//   auto            err = clfftCreateDefaultPlan(&planHandle, ctx, dim, clLengths.data());

//   /* Set plan parameters. */
//   err = clfftSetPlanPrecision(planHandle, CLFFT_SINGLE);
//   err = clfftSetLayout(planHandle, CLFFT_REAL, CLFFT_HERMITIAN_INTERLEAVED);
//   err = clfftSetResultLocation(planHandle, CLFFT_OUTOFPLACE);
//   err = clfftSetPlanInStride(planHandle, dim, inStride.data());
//   err = clfftSetPlanOutStride(planHandle, dim, outStride.data());

//   /* Bake the plan. */
//   err = clfftBakePlan(planHandle, 1, &queue, NULL, NULL);

//   /* Return the plan */
//   return planHandle;
// }


// auto
// bake_backward(const Array::Pointer & real) -> clfftPlanHandle
// {
//   auto ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(real->device());
//   auto ctx = ocl_device->getCLContext();
//   auto queue = ocl_device->getCLCommandQueue();

//   /* FFT library related declarations */
//   clfftDim            dim;
//   std::vector<size_t> clLengths;
//   std::vector<size_t> inStride;
//   std::vector<size_t> outStride;
//   size_t              hermitian_width = static_cast<size_t>(real->width() / 2) + 1;
//   // Reserve space for the vectors
//   clLengths.reserve(3);
//   inStride.reserve(3);
//   outStride.reserve(3);
//   switch (real->dimension())
//   {
//     case 1:
//       dim = CLFFT_1D;
//       clLengths = { real->width() };
//       inStride = { 1 };
//       outStride = { 1 };
//       break;
//     case 2:
//       dim = CLFFT_2D;
//       clLengths = { real->width(), real->height() };
//       inStride = { 1, hermitian_width };
//       outStride = { 1, real->width() };
//       break;
//     case 3:
//       dim = CLFFT_3D;
//       clLengths = { real->width(), real->height(), real->depth() };
//       inStride = { 1, hermitian_width, hermitian_width * real->height() };
//       outStride = { 1, real->width(), real->width() * real->height() };
//       break;
//     default:
//       throw std::runtime_error("Invalid FFT dimension");
//   }

//   /* Create a default plan for a complex FFT. */
//   clfftPlanHandle planHandle;
//   auto            err = clfftCreateDefaultPlan(&planHandle, ctx, dim, clLengths.data());

//   /* Set plan parameters. */
//   err = clfftSetPlanPrecision(planHandle, CLFFT_SINGLE);
//   err = clfftSetLayout(planHandle, CLFFT_HERMITIAN_INTERLEAVED, CLFFT_REAL);
//   err = clfftSetResultLocation(planHandle, CLFFT_OUTOFPLACE);
//   err = clfftSetPlanInStride(planHandle, dim, inStride.data());
//   err = clfftSetPlanOutStride(planHandle, dim, outStride.data());

//   /* Bake the plan. */
//   err = clfftBakePlan(planHandle, 1, &queue, NULL, NULL);

//   /* Return the plan */
//   return planHandle;
// }


auto
execOperationKernel(const Device::Pointer & device,
                    const std::string       name,
                    const Array::Pointer &  inputA,
                    const Array::Pointer &  inputB,
                    Array::Pointer          output,
                    const unsigned int      nElements) -> Array::Pointer
{
  constexpr size_t LOCAL_ITEM_SIZE = 64;
  size_t           globalItemSize =
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
  constexpr size_t LOCAL_ITEM_SIZE = 64;
  size_t           globalItemSize =
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

  const RangeArray    global_range = { static_cast<size_t>(nx), static_cast<size_t>(ny), static_cast<size_t>(nz) };
  const RangeArray    local_range = { 512, 512, 64 };
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


// auto
// fft_forward(const Array::Pointer & real, Array::Pointer complex) -> Array::Pointer
// {
//   auto ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(real->device());
//   auto ctx = ocl_device->getCLContext();
//   auto queue = ocl_device->getCLCommandQueue();

//   if (complex == nullptr)
//   {
//     complex = create_hermitian(real);
//   }

//   /* Setup clFFT. */
//   auto err = SetupFFT();

//   /* FFT library related declarations */
//   auto planHandle = bake_forward(real);

//   /* Execute the plan. */
//   err = clfftEnqueueTransform(planHandle,
//                               CLFFT_FORWARD,
//                               1,
//                               &queue,
//                               0,
//                               NULL,
//                               NULL,
//                               static_cast<cl_mem *>(*real->get()),
//                               static_cast<cl_mem *>(*complex->get()),
//                               NULL);

//   /* Wait for calculations to be finished. */
//   err = clFinish(queue);

//   /* Release the plan. */
//   err = clfftDestroyPlan(&planHandle);

//   /* Release clFFT library. */
//   clfftTeardown();

//   return complex;
// }


// auto
// fft_backward(const Array::Pointer & complex, Array::Pointer real) -> void
// {
//   auto ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(complex->device());
//   auto ctx = ocl_device->getCLContext();
//   auto queue = ocl_device->getCLCommandQueue();

//   /* Setup clFFT. */
//   auto err = SetupFFT();

//   /* FFT library related declarations */
//   auto planHandle = bake_backward(real);

//   /* Execute the plan. */
//   err = clfftEnqueueTransform(planHandle,
//                               CLFFT_BACKWARD,
//                               1,
//                               &queue,
//                               0,
//                               NULL,
//                               NULL,
//                               static_cast<cl_mem *>(*complex->get()),
//                               static_cast<cl_mem *>(*real->get()),
//                               NULL);

//   /* Wait for calculations to be finished. */
//   err = clFinish(queue);

//   /* Release the plan. */
//   err = clfftDestroyPlan(&planHandle);

//   /* Release clFFT library. */
//   clfftTeardown();
// }


// auto
// convolution(const Array::Pointer & input, const Array::Pointer & psf, Array::Pointer output, bool correlate)
//   -> Array::Pointer
// {
//   auto ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(input->device());
//   auto ctx = ocl_device->getCLContext();
//   auto queue = ocl_device->getCLCommandQueue();

//   size_t nElts = input->size();
//   size_t nFreq = (input->width() / 2 + 1) * input->height() * input->depth();

//   // create output gpu buffer if not provided
//   if (output == nullptr)
//   {
//     output = Array::create(input);
//   }

//   // create fft of psf and input
//   auto fft_psf = create_hermitian(psf);
//   auto fft_input = create_hermitian(input);

//   // create forward and backward plans
//   auto forward_plan = bake_forward(input);
//   auto backward_plan = bake_backward(input);

//   // FFT of PSF
//   clfftEnqueueTransform(forward_plan,
//                         CLFFT_FORWARD,
//                         1,
//                         &queue,
//                         0,
//                         NULL,
//                         NULL,
//                         static_cast<cl_mem *>(*psf->get()),
//                         static_cast<cl_mem *>(*fft_psf->get()),
//                         NULL);
//   // FFT of image
//   clfftEnqueueTransform(forward_plan,
//                         CLFFT_FORWARD,
//                         1,
//                         &queue,
//                         0,
//                         NULL,
//                         NULL,
//                         static_cast<cl_mem *>(*input->get()),
//                         static_cast<cl_mem *>(*fft_input->get()),
//                         NULL);

//   // Multiply in frequency domain
//   constexpr size_t LOCAL_ITEM_SIZE = 64;
//   size_t           globalItemSize =
//     static_cast<size_t>(ceil(static_cast<double>(nElts) / static_cast<double>(LOCAL_ITEM_SIZE)) * LOCAL_ITEM_SIZE);
//   size_t globalItemSizeFreq =
//     static_cast<size_t>(ceil(static_cast<double>(nFreq) / static_cast<double>(LOCAL_ITEM_SIZE)) * LOCAL_ITEM_SIZE);
//   std::string kernel_name = correlate ? "vecComplexConjugateMultiply" : "vecComplexMultiply";
//   execOperationKernel(
//     kernel_name, fft_input, fft_psf, fft_input, nFreq, { globalItemSizeFreq, 1, 1 }, { LOCAL_ITEM_SIZE, 1, 1 });

//   // Inverse to get convolved
//   clfftEnqueueTransform(backward_plan,
//                         CLFFT_BACKWARD,
//                         1,
//                         &queue,
//                         0,
//                         NULL,
//                         NULL,
//                         static_cast<cl_mem *>(*fft_input->get()),
//                         static_cast<cl_mem *>(*output->get()),
//                         NULL);

//   // Release the plan.
//   clfftDestroyPlan(&forward_plan);
//   clfftDestroyPlan(&backward_plan);

//   /* Release clFFT library. */
//   clfftTeardown();

//   return output;
// }


// auto
// deconvolution(const Array::Pointer & observe,
//               const Array::Pointer & psf,
//               Array::Pointer         normal,
//               Array::Pointer         estimate,
//               size_t                 iterations,
//               float                  regularization) -> Array::Pointer
// {
//   // fetch ocl device, context and queue
//   auto ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(observe->device());
//   auto ctx = ocl_device->getCLContext();
//   auto queue = ocl_device->getCLCommandQueue();

//   // get the number of elements and frequency domain elements
//   size_t nElts = observe->size();
//   size_t nFreq = (observe->width() / 2 + 1) * observe->height() * observe->depth();

//   // set the global and local item sizes
//   constexpr size_t LOCAL_ITEM_SIZE = 64;
//   size_t           globalItemSize =
//     static_cast<size_t>(ceil(static_cast<double>(nElts) / static_cast<double>(LOCAL_ITEM_SIZE)) * LOCAL_ITEM_SIZE);
//   size_t globalItemSizeFreq = static_cast<size_t>(
//     ceil(static_cast<double>(nFreq + 1000) / static_cast<double>(LOCAL_ITEM_SIZE)) * LOCAL_ITEM_SIZE);

//   // check if total variation is used
//   bool use_tv = regularization > 0;

//   // create buffers for the computation
//   if (estimate == nullptr)
//   {
//     estimate = Array::create(observe);
//   }
//   auto           reblurred = Array::create(observe);
//   auto           fft_estimate = create_hermitian(estimate);
//   auto           fft_psf = create_hermitian(psf);
//   Array::Pointer variation = nullptr;
//   if (use_tv)
//   {
//     variation = Array::create(observe);
//   }

//   // create forward and backward plans
//   auto forward_plan = bake_forward(observe);
//   auto backward_plan = bake_backward(observe);

//   // FFT of PSF
//   clfftEnqueueTransform(forward_plan,
//                         CLFFT_FORWARD,
//                         1,
//                         &queue,
//                         0,
//                         NULL,
//                         NULL,
//                         static_cast<cl_mem *>(*psf->get()),
//                         static_cast<cl_mem *>(*fft_psf->get()),
//                         NULL);

//   if (normal != nullptr)
//   {
//     // remove small values from normal, if used
//     execRemoveSmallValues(normal, nElts, { globalItemSize, 1, 1 }, { LOCAL_ITEM_SIZE, 1, 1 });
//   }


//   // Richardson Lucy - deconvolution loop
//   for (size_t i = 0; i < iterations; i++)
//   {
//     // FFT of estimate
//     clfftEnqueueTransform(forward_plan,
//                           CLFFT_FORWARD,
//                           1,
//                           &queue,
//                           0,
//                           NULL,
//                           NULL,
//                           static_cast<cl_mem *>(*estimate->get()),
//                           static_cast<cl_mem *>(*fft_estimate->get()),
//                           NULL);

//     // complex multiply estimate FFT and PSF FFT
//     execOperationKernel("vecComplexMultiply",
//                         fft_estimate,
//                         fft_psf,
//                         fft_estimate,
//                         nFreq,
//                         { globalItemSizeFreq, 1, 1 },
//                         { LOCAL_ITEM_SIZE, 1, 1 });

//     // Inverse to get reblurred
//     clfftEnqueueTransform(backward_plan,
//                           CLFFT_BACKWARD,
//                           1,
//                           &queue,
//                           0,
//                           NULL,
//                           NULL,
//                           static_cast<cl_mem *>(*fft_estimate->get()),
//                           static_cast<cl_mem *>(*reblurred->get()),
//                           NULL);

//     // divide observed by reblurred
//     execOperationKernel(
//       "vecDiv", observe, reblurred, reblurred, nElts, { globalItemSize, 1, 1 }, { LOCAL_ITEM_SIZE, 1, 1 });

//     // FFT of observed/reblurred
//     clfftEnqueueTransform(forward_plan,
//                           CLFFT_FORWARD,
//                           1,
//                           &queue,
//                           0,
//                           NULL,
//                           NULL,
//                           static_cast<cl_mem *>(*reblurred->get()),
//                           static_cast<cl_mem *>(*fft_estimate->get()),
//                           NULL);

//     // Correlate above result with PSF
//     execOperationKernel("vecComplexConjugateMultiply",
//                         fft_estimate,
//                         fft_psf,
//                         fft_estimate,
//                         nFreq,
//                         { globalItemSizeFreq, 1, 1 },
//                         { LOCAL_ITEM_SIZE, 1, 1 });

//     // Inverse FFT to get update factor
//     clfftEnqueueTransform(backward_plan,
//                           CLFFT_BACKWARD,
//                           1,
//                           &queue,
//                           0,
//                           NULL,
//                           NULL,
//                           static_cast<cl_mem *>(*fft_estimate->get()),
//                           static_cast<cl_mem *>(*reblurred->get()),
//                           NULL);

//     // total variation multiply by variation factor, if used
//     if (use_tv)
//     {
//       // calculate total variation
//       execTotalVariationTerm(estimate,
//                              reblurred,
//                              variation,
//                              observe->width(),
//                              observe->height(),
//                              observe->depth(),
//                              1.0,
//                              1.0,
//                              3.0,
//                              regularization,
//                              { globalItemSize, 1, 1 },
//                              { LOCAL_ITEM_SIZE, 1, 1 });
//       // multiply by variation factor
//       execOperationKernel(
//         "vecMul", estimate, variation, estimate, nElts, { globalItemSize, 1, 1 }, { LOCAL_ITEM_SIZE, 1, 1 });
//     }
//     else
//     {
//       // multiply estimate by update factor
//       execOperationKernel(
//         "vecMul", estimate, reblurred, estimate, nElts, { globalItemSize, 1, 1 }, { LOCAL_ITEM_SIZE, 1, 1 });
//     }

//     if (normal != nullptr)
//     {
//       // divide estimate by normal, if used
//       execOperationKernel(
//         "vecDiv", estimate, normal, estimate, nElts, { globalItemSize, 1, 1 }, { LOCAL_ITEM_SIZE, 1, 1 });
//     }

//     // wait for calculations to be finished before next iteration
//     clFinish(queue);
//   }

//   // Release the plan.
//   clfftDestroyPlan(&forward_plan);
//   clfftDestroyPlan(&backward_plan);

//   return estimate;
// }


} // namespace cle::fft
