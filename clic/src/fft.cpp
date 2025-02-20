
//
// This work is adapted from the clij2-fft repository <https://github.com/clij/clij2-fft/tree/master>
// from the work of Brian Northan <https://github.com/bnorthan> under the BSD-3-Clause License
//


#include "fft.hpp"
#include "device.hpp"

#include <array>
#include <stdexcept>

#include "cle_fft.h"

namespace cle::fft
{

auto
SetupFFT() -> cl_int
{
  clfftSetupData fftSetup;
  cl_int         err = clfftInitSetupData(&fftSetup);
  err = clfftSetup(&fftSetup);
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Failed to setup clFFT");
  }
  return err;
}

auto
bake_forward(const Array::Pointer & real) -> clfftPlanHandle
{
  auto ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(real->device());
  auto ctx = ocl_device->getCLContext();
  auto queue = ocl_device->getCLCommandQueue();

  /* FFT library realted declarations */
  constexpr size_t               dimensions = 3;
  std::array<size_t, dimensions> clLengths = { real->width(), real->height(), real->depth() };
  clfftDim                       dim;
  std::array<size_t, dimensions> inStride;
  std::array<size_t, dimensions> outStride;
  size_t hermitian_width = static_cast<size_t>(real->width() / 2 + 1);
  switch (real->dimension())
  {
    case 1:
      dim = CLFFT_1D;
      inStride = { 1, 1, 1 };
      outStride = { 1, 1, 1 };
      break;
    case 2:
      dim = CLFFT_2D;
      inStride = { 1, static_cast<size_t>(real->width()), 1 };
      outStride = { 1, hermitian_width, 1 };
      break;
    case 3:
      dim = CLFFT_3D;
      inStride = { 1, static_cast<size_t>(real->width()), static_cast<size_t>(real->width()) * real->height() };
      outStride = { 1, hermitian_width, hermitian_width * real->height() };
      break;
    default:
      throw std::runtime_error("Invalid FFT dimension");
  }

  /* Create a default plan for a complex FFT. */
  clfftPlanHandle planHandle;
  auto            err = clfftCreateDefaultPlan(&planHandle, ctx, dim, clLengths.data());

  /* Set plan parameters. */
  err = clfftSetPlanPrecision(planHandle, CLFFT_SINGLE_FAST);
  err = clfftSetLayout(planHandle, CLFFT_REAL, CLFFT_HERMITIAN_INTERLEAVED);
  err = clfftSetResultLocation(planHandle, CLFFT_OUTOFPLACE);
  err = clfftSetPlanInStride(planHandle, dim, inStride.data());
  err = clfftSetPlanOutStride(planHandle, dim, outStride.data());

  /* Bake the plan. */
  err = clfftBakePlan(planHandle, 1, &queue, NULL, NULL);

  /* Return the plan */
  return planHandle;
}


auto
bake_backward(const Array::Pointer & real) -> clfftPlanHandle
{
  auto ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(real->device());
  auto ctx = ocl_device->getCLContext();
  auto queue = ocl_device->getCLCommandQueue();

  /* FFT library realted declarations */
  constexpr size_t               dimensions = 3;
  std::array<size_t, dimensions> clLengths = { real->width(), real->height(), real->depth() };
  clfftDim                       dim;
  std::array<size_t, dimensions> inStride;
  std::array<size_t, dimensions> outStride;
  size_t hermitian_width = static_cast<size_t>(real->width() / 2 + 1);
  switch (real->dimension())
  {
    case 1:
      dim = CLFFT_1D;
      inStride = { 1, 1, 1 };
      outStride = { 1, 1, 1 };
      break;
    case 2:
      dim = CLFFT_2D;
      inStride = { 1, hermitian_width, 1 };
      outStride = { 1, static_cast<size_t>(real->width()), 1 };
      break;
    case 3:
      dim = CLFFT_3D;
      inStride = { 1, hermitian_width, hermitian_width * real->height() };
      outStride = { 1, static_cast<size_t>(real->width()), static_cast<size_t>(real->width()) * real->height() };
      break;
    default:
      throw std::runtime_error("Invalid FFT dimension");
  }

  /* Create a default plan for a complex FFT. */
  clfftPlanHandle planHandle;
  auto            err = clfftCreateDefaultPlan(&planHandle, ctx, dim, clLengths.data());

  /* Set plan parameters. */
  err = clfftSetPlanPrecision(planHandle, CLFFT_SINGLE_FAST);
  err = clfftSetLayout(planHandle, CLFFT_HERMITIAN_INTERLEAVED, CLFFT_REAL);
  err = clfftSetResultLocation(planHandle, CLFFT_OUTOFPLACE);
  err = clfftSetPlanInStride(planHandle, dim, inStride.data());
  err = clfftSetPlanOutStride(planHandle, dim, outStride.data());

  /* Bake the plan. */
  err = clfftBakePlan(planHandle, 1, &queue, NULL, NULL);

  /* Return the plan */
  return planHandle;
}


Array::Pointer
create_hermitian(const Array::Pointer & real_buf)
{
  size_t hermitian_width = static_cast<size_t>(real_buf->width() / 2 + 1);
  size_t nFreq = real_buf->depth() * real_buf->height() * (hermitian_width);

  auto ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(real_buf->device());

  // create a new buffer with twice the width for the imaginary part
  return Array::create(hermitian_width * 2,
                       real_buf->height(),
                       real_buf->depth(),
                       real_buf->dimension(),
                       real_buf->dtype(),
                       real_buf->mtype(),
                       real_buf->device());
}

auto
execOperationKernel(const std::string      name,
                    const Array::Pointer & bufferA,
                    const Array::Pointer & bufferB,
                    const Array::Pointer & buffer_out,
                    size_t                 nElements,
                    const RangeArray &     global_range,
                    const RangeArray &     local_range) -> void
{
  auto                ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(bufferA->device());
  KernelInfo          kernel = { name, kernel::fft };
  const ParameterList params = { { "a", bufferA }, { "b", bufferB }, { "c", buffer_out }, { "n", nElements } };
  native_execute(ocl_device, kernel, params, global_range, local_range);
}

auto
execRemoveSmallValues(Array::Pointer &   BufferA,
                      size_t             nElements,
                      const RangeArray & global_range,
                      const RangeArray & local_range) -> void
{
  auto                ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(BufferA->device());
  KernelInfo          kernel = { "removeSmallValues", kernel::fft };
  const ParameterList params = { { "a", BufferA }, { "n", nElements } };
  native_execute(ocl_device, kernel, params, global_range, local_range);
}

auto
execTotalVariationTerm(const Array::Pointer & estimate,
                       const Array::Pointer & correction,
                       const Array::Pointer & variation,
                       size_t                 nx,
                       size_t                 ny,
                       size_t                 nz,
                       float                  hx,
                       float                  hy,
                       float                  hz,
                       float                  regularization_factor,
                       const RangeArray &     global_range,
                       const RangeArray &     local_range) -> void
{
  auto                ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(estimate->device());
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
  native_execute(ocl_device, kernel, params, global_range, local_range);
}


auto
fft_forward(const Array::Pointer & real, Array::Pointer complex) -> Array::Pointer
{
  auto ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(real->device());
  auto ctx = ocl_device->getCLContext();
  auto queue = ocl_device->getCLCommandQueue();

  

  if (complex == nullptr)
  {
    complex = create_hermitian(real);
  }

  /* Setup clFFT. */
  auto err = SetupFFT();

  /* FFT library realted declarations */
  auto planHandle = bake_forward(real);

  /* Execute the plan. */
  err = clfftEnqueueTransform(planHandle,
                              CLFFT_FORWARD,
                              1,
                              &queue,
                              0,
                              NULL,
                              NULL,
                              static_cast<cl_mem *>(*real->get()),
                              static_cast<cl_mem *>(*complex->get()),
                              NULL);

  /* Wait for calculations to be finished. */
  err = clFinish(queue);

  /* Release the plan. */
  err = clfftDestroyPlan(&planHandle);

  /* Release clFFT library. */
  clfftTeardown();

  return complex;
}


auto
fft_backward(const Array::Pointer & complex, Array::Pointer real) -> void
{
  auto ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(complex->device());
  auto ctx = ocl_device->getCLContext();
  auto queue = ocl_device->getCLCommandQueue();

  /* Setup clFFT. */
  auto err = SetupFFT();

  /* FFT library related declarations */
  auto planHandle = bake_backward(real);

  /* Execute the plan. */
  err = clfftEnqueueTransform(planHandle,
                              CLFFT_BACKWARD,
                              1,
                              &queue,
                              0,
                              NULL,
                              NULL,
                              static_cast<cl_mem *>(*complex->get()),
                              static_cast<cl_mem *>(*real->get()),
                              NULL);

  /* Wait for calculations to be finished. */
  err = clFinish(queue);

  /* Release the plan. */
  err = clfftDestroyPlan(&planHandle);

  /* Release clFFT library. */
  clfftTeardown();
}


auto
convolution(const Array::Pointer & input, const Array::Pointer & psf, Array::Pointer output, bool correlate)
  -> Array::Pointer
{
  auto ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(input->device());
  auto ctx = ocl_device->getCLContext();
  auto queue = ocl_device->getCLCommandQueue();

  size_t nElts = input->size();
  size_t nFreq = (input->width() / 2 + 1) * input->height() * input->depth();

  // create output gpu buffer if not provided
  if (output == nullptr)
  {
    output = Array::create(input);
  }

  // create fft of psf and input
  auto fft_psf = create_hermitian(psf);
  auto fft_input = create_hermitian(input);

  // create forward and backward plans
  auto forward_plan = bake_forward(input);
  auto backward_plan = bake_backward(input);

  // FFT of PSF
  clfftEnqueueTransform(forward_plan,
                        CLFFT_FORWARD,
                        1,
                        &queue,
                        0,
                        NULL,
                        NULL,
                        static_cast<cl_mem *>(*psf->get()),
                        static_cast<cl_mem *>(*fft_psf->get()),
                        NULL);
  // FFT of image
  clfftEnqueueTransform(forward_plan,
                        CLFFT_FORWARD,
                        1,
                        &queue,
                        0,
                        NULL,
                        NULL,
                        static_cast<cl_mem *>(*input->get()),
                        static_cast<cl_mem *>(*fft_input->get()),
                        NULL);

  // Multiply in frequency domain
  constexpr size_t LOCAL_ITEM_SIZE = 64;
  size_t           globalItemSize =
    static_cast<size_t>(ceil(static_cast<double>(nElts) / static_cast<double>(LOCAL_ITEM_SIZE)) * LOCAL_ITEM_SIZE);
  size_t globalItemSizeFreq =
    static_cast<size_t>(ceil(static_cast<double>(nFreq) / static_cast<double>(LOCAL_ITEM_SIZE)) * LOCAL_ITEM_SIZE);
  std::string kernel_name = correlate ? "vecComplexConjugateMultiply" : "vecComplexMultiply";
  execOperationKernel(
    kernel_name, fft_input, fft_psf, fft_input, nFreq, { globalItemSizeFreq, 1, 1 }, { LOCAL_ITEM_SIZE, 1, 1 });

  // Inverse to get convolved
  clfftEnqueueTransform(backward_plan,
                        CLFFT_BACKWARD,
                        1,
                        &queue,
                        0,
                        NULL,
                        NULL,
                        static_cast<cl_mem *>(*fft_input->get()),
                        static_cast<cl_mem *>(*output->get()),
                        NULL);

  // Release the plan.
  clfftDestroyPlan(&forward_plan);
  clfftDestroyPlan(&backward_plan);

  /* Release clFFT library. */
  clfftTeardown();

  return output;
}


auto
deconvolution(const Array::Pointer & observe,
              const Array::Pointer & psf,
              Array::Pointer         normal,
              Array::Pointer         estimate,
              size_t                 iterations,
              float                  regularization) -> Array::Pointer
{
  // fetch ocl device, context and queue
  auto ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(observe->device());
  auto ctx = ocl_device->getCLContext();
  auto queue = ocl_device->getCLCommandQueue();

  // get the number of elements and frequency domain elements
  size_t nElts = observe->size();
  size_t nFreq = (observe->width() / 2 + 1) * observe->height() * observe->depth();

  // set the global and local item sizes
  constexpr size_t LOCAL_ITEM_SIZE = 64;
  size_t           globalItemSize =
    static_cast<size_t>(ceil(static_cast<double>(nElts) / static_cast<double>(LOCAL_ITEM_SIZE)) * LOCAL_ITEM_SIZE);
  size_t globalItemSizeFreq = static_cast<size_t>(
    ceil(static_cast<double>(nFreq + 1000) / static_cast<double>(LOCAL_ITEM_SIZE)) * LOCAL_ITEM_SIZE);

  // check if total variation is used
  bool use_tv = regularization > 0;

  // create buffers for the computation
  if (estimate == nullptr)
  {
    estimate = Array::create(observe);
  }
  auto           reblurred = Array::create(observe);
  auto           fft_estimate = create_hermitian(estimate);
  auto           fft_psf = create_hermitian(psf);
  Array::Pointer variation = nullptr;
  if (use_tv)
  {
    variation = Array::create(observe);
  }

  // create forward and backward plans
  auto forward_plan = bake_forward(observe);
  auto backward_plan = bake_backward(observe);

  // FFT of PSF
  clfftEnqueueTransform(forward_plan,
                        CLFFT_FORWARD,
                        1,
                        &queue,
                        0,
                        NULL,
                        NULL,
                        static_cast<cl_mem *>(*psf->get()),
                        static_cast<cl_mem *>(*fft_psf->get()),
                        NULL);

  if (normal != nullptr)
  {
    // remove small values from normal, if used
    execRemoveSmallValues(normal, nElts, { globalItemSize, 1, 1 }, { LOCAL_ITEM_SIZE, 1, 1 });
  }


  // Richardson Lucy - deconvolution loop
  for (size_t i = 0; i < iterations; i++)
  {
    // FFT of estimate
    clfftEnqueueTransform(forward_plan,
                          CLFFT_FORWARD,
                          1,
                          &queue,
                          0,
                          NULL,
                          NULL,
                          static_cast<cl_mem *>(*estimate->get()),
                          static_cast<cl_mem *>(*fft_estimate->get()),
                          NULL);

    // complex multiply estimate FFT and PSF FFT
    execOperationKernel("vecComplexMultiply",
                        fft_estimate,
                        fft_psf,
                        fft_estimate,
                        nFreq,
                        { globalItemSizeFreq, 1, 1 },
                        { LOCAL_ITEM_SIZE, 1, 1 });

    // Inverse to get reblurred
    clfftEnqueueTransform(backward_plan,
                          CLFFT_BACKWARD,
                          1,
                          &queue,
                          0,
                          NULL,
                          NULL,
                          static_cast<cl_mem *>(*fft_estimate->get()),
                          static_cast<cl_mem *>(*reblurred->get()),
                          NULL);

    // divide observed by reblurred
    execOperationKernel(
      "vecDiv", observe, reblurred, reblurred, nElts, { globalItemSize, 1, 1 }, { LOCAL_ITEM_SIZE, 1, 1 });

    // FFT of observed/reblurred
    clfftEnqueueTransform(forward_plan,
                          CLFFT_FORWARD,
                          1,
                          &queue,
                          0,
                          NULL,
                          NULL,
                          static_cast<cl_mem *>(*reblurred->get()),
                          static_cast<cl_mem *>(*fft_estimate->get()),
                          NULL);

    // Correlate above result with PSF
    execOperationKernel("vecComplexConjugateMultiply",
                        fft_estimate,
                        fft_psf,
                        fft_estimate,
                        nFreq,
                        { globalItemSizeFreq, 1, 1 },
                        { LOCAL_ITEM_SIZE, 1, 1 });

    // Inverse FFT to get update factor
    clfftEnqueueTransform(backward_plan,
                          CLFFT_BACKWARD,
                          1,
                          &queue,
                          0,
                          NULL,
                          NULL,
                          static_cast<cl_mem *>(*fft_estimate->get()),
                          static_cast<cl_mem *>(*reblurred->get()),
                          NULL);

    // total variation multiply by variation factor, if used
    if (use_tv)
    {
      // calculate total variation
      execTotalVariationTerm(estimate,
                             reblurred,
                             variation,
                             observe->width(),
                             observe->height(),
                             observe->depth(),
                             1.0,
                             1.0,
                             3.0,
                             regularization,
                             { globalItemSize, 1, 1 },
                             { LOCAL_ITEM_SIZE, 1, 1 });
      // multiply by variation factor
      execOperationKernel(
        "vecMul", estimate, variation, estimate, nElts, { globalItemSize, 1, 1 }, { LOCAL_ITEM_SIZE, 1, 1 });
    }
    else
    {
      // multiply estimate by update factor
      execOperationKernel(
        "vecMul", estimate, reblurred, estimate, nElts, { globalItemSize, 1, 1 }, { LOCAL_ITEM_SIZE, 1, 1 });
    }

    if (normal != nullptr)
    {
      // divide estimate by normal, if used
      execOperationKernel(
        "vecDiv", estimate, normal, estimate, nElts, { globalItemSize, 1, 1 }, { LOCAL_ITEM_SIZE, 1, 1 });
    }

    // wait for calculations to be finished before next iteration
    clFinish(queue);
  }

  // Release the plan.
  clfftDestroyPlan(&forward_plan);
  clfftDestroyPlan(&backward_plan);

  return estimate;
}


} // namespace cle::fft
