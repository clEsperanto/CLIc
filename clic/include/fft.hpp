#ifndef __INCLUDE_FFT_HPP
#define __INCLUDE_FFT_HPP

#include "clFFT.h"

#include "array.hpp"
#include "backend.hpp"
#include "device.hpp"
#include "execution.hpp"

namespace cle::fft
{


clfftDim
get_dim(Array::Pointer array)
{
  switch (array->dimension())
  {
    case 1:
      std::cout << "CLFFT_1D" << std::endl;
      return CLFFT_1D;
    case 2:
      std::cout << "CLFFT_2D" << std::endl;
      return CLFFT_2D;
    case 3:
      std::cout << "CLFFT_3D" << std::endl;
      return CLFFT_3D;
    default:
      throw std::runtime_error("Invalid dimension");
  }
  return CLFFT_1D;
}


Array::Pointer
create_complex_buffer(const Array::Pointer & data)
{
  cl_int err;
  auto   ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(data->device());

  // create a new buffer with twice the width for the imaginary part
  return Array::create(
    data->width() * 2, data->height(), data->depth(), data->dimension(), data->dtype(), data->mtype(), data->device());

}

Array::Pointer
create_real_buffer(const Array::Pointer & data)
{
  cl_int err;
  auto   ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(data->device());

  // create a new buffer with twice the width for the imaginary part
  return Array::create(
    data->width() / 2, data->height(), data->depth(), data->dimension(), data->dtype(), data->mtype(), data->device());

}


clfftPlanHandle bake_forward(const Array::Pointer & data)
{
  auto ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(data->device());
  auto ctx = ocl_device->getCLContext();
  auto queue = ocl_device->getCLCommandQueue();

  /* Setup clFFT. */
  clfftSetupData fftSetup;
  cl_int err = clfftInitSetupData(&fftSetup);
  err = clfftSetup(&fftSetup);

  /* FFT library realted declarations */
  clfftPlanHandle planHandle;
  size_t          clLengths[] = { data->width(), data->height(), data->depth() };
  clfftDim dim;
  size_t inStride[3];
  size_t outStride[3];
  switch (data->dimension())  
  {
    case 1:
    dim = CLFFT_1D;
    inStride[0] = 1; inStride[1] = 1; inStride[2] = 1;
    outStride[0] = 1; outStride[1] = 1; outStride[2] = 1;
    break;
    case 2:
    dim = CLFFT_2D;
    inStride[0] = 1; inStride[1] = (size_t)data->width(); inStride[2] = 1;
    outStride[0] = 1; outStride[1] = (size_t)data->width()/2+1; outStride[2] = 1;
    break;
    case 3:
    dim = CLFFT_3D;
    inStride[0] = 1; inStride[1] = (size_t)data->width(); inStride[2] = (size_t)data->width() * data->height();
    outStride[0] = 1; outStride[1] = (size_t)data->width()/2+1; outStride[2] = ((size_t)data->width()/2+1) * data->height();
    break;
    default:
    throw std::runtime_error("Invalid FFT dimension");
  }
  
  /* Create a default plan for a complex FFT. */
  err = clfftCreateDefaultPlan(&planHandle, ctx, dim, clLengths);

  /* Set plan parameters. */
  err = clfftSetPlanPrecision(planHandle, CLFFT_SINGLE);
  err = clfftSetLayout(planHandle, CLFFT_REAL, CLFFT_HERMITIAN_INTERLEAVED);
  err = clfftSetResultLocation(planHandle, CLFFT_OUTOFPLACE);
  err = clfftSetPlanInStride(planHandle, dim, inStride);
  err = clfftSetPlanOutStride(planHandle, dim, outStride);

  /* Bake the plan. */
  err = clfftBakePlan(planHandle, 1, &queue, NULL, NULL);

  return planHandle;
}


clfftPlanHandle bake_backward(const Array::Pointer & data)
{
  auto ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(data->device());
  auto ctx = ocl_device->getCLContext();
  auto queue = ocl_device->getCLCommandQueue();

  /* Setup clFFT. */
  clfftSetupData fftSetup;
  cl_int err = clfftInitSetupData(&fftSetup);
  err = clfftSetup(&fftSetup);

  /* FFT library realted declarations */
  
  
  clfftPlanHandle planHandle;
  size_t          clLengths[] = { data->width(), data->height(), data->depth() };
  clfftDim dim;
  size_t inStride[3];
  size_t outStride[3];
  switch (data->dimension())  
  {
    case 1:
    dim = CLFFT_1D;
    inStride[0] = 1; inStride[1] = 1; inStride[2] = 1;
    outStride[0] = 1; outStride[1] = 1; outStride[2] = 1;
    break;
    case 2:
    dim = CLFFT_2D;
    inStride[0] = 1; inStride[1] = (size_t)data->width()/2+1; inStride[2] = 1;
    outStride[0] = 1; outStride[1] = (size_t)data->width(); outStride[2] = 1;
    break;
    case 3:
    dim = CLFFT_3D;
    inStride[0] = 1; inStride[1] = (size_t)data->width()/2+1; inStride[2] = ((size_t)data->width()/2+1) * data->height();
    outStride[0] = 1; outStride[1] = (size_t)data->width(); outStride[2] = (size_t)data->width() * data->height();
    break;
    default:
    throw std::runtime_error("Invalid FFT dimension");
  }
  
  
  /* Create a default plan for a complex FFT. */
  err = clfftCreateDefaultPlan(&planHandle, ctx, dim, clLengths);

  /* Set plan parameters. */
  err = clfftSetPlanPrecision(planHandle, CLFFT_SINGLE);
  err = clfftSetLayout(planHandle, CLFFT_HERMITIAN_INTERLEAVED, CLFFT_REAL);
  err = clfftSetResultLocation(planHandle, CLFFT_OUTOFPLACE);
  err = clfftSetPlanInStride(planHandle, dim, inStride);
  err = clfftSetPlanOutStride(planHandle, dim, outStride);

  /* Bake the plan. */
  err = clfftBakePlan(planHandle, 1, &queue, NULL, NULL);

  return planHandle;
}



Array::Pointer fft_forward(const Array::Pointer & input, Array::Pointer output)
{

  auto ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(input->device());
  auto ctx = ocl_device->getCLContext();
  auto queue = ocl_device->getCLCommandQueue();

  if (output == nullptr)
  {
    output = create_complex_buffer(input);
  }

  /* Setup clFFT. */
  clfftSetupData fftSetup;
  cl_int err = clfftInitSetupData(&fftSetup);
  err = clfftSetup(&fftSetup);

  /* FFT library realted declarations */
  auto planHandle = bake_forward(input);

  /* Bake the plan. */
  err = clfftBakePlan(planHandle, 1, &queue, NULL, NULL);

  /* Execute the plan. */
  err = clfftEnqueueTransform(
    planHandle, CLFFT_FORWARD, 1, &queue, 0, NULL, NULL, static_cast<cl_mem *>(*input->get()), static_cast<cl_mem *>(*output->get()), NULL);

  /* Wait for calculations to be finished. */
  err = clFinish(queue);

  /* Release the plan. */
  err = clfftDestroyPlan(&planHandle);

  /* Release clFFT library. */
  clfftTeardown();

  return output;
}


Array::Pointer fft_backward(const Array::Pointer & input, Array::Pointer output)
{

  auto ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(input->device());
  auto ctx = ocl_device->getCLContext();
  auto queue = ocl_device->getCLCommandQueue();

  if (output == nullptr)
  {
    output = create_real_buffer(input);
  }

  /* Setup clFFT. */
  clfftSetupData fftSetup;
  cl_int err = clfftInitSetupData(&fftSetup);
  err = clfftSetup(&fftSetup);

  /* FFT library realted declarations */
  auto planHandle = bake_backward(output);

  /* Execute the plan. */
  err = clfftEnqueueTransform(
    planHandle, CLFFT_BACKWARD, 1, &queue, 0, NULL, NULL, static_cast<cl_mem *>(*input->get()), static_cast<cl_mem *>(*output->get()), NULL);

  /* Wait for calculations to be finished. */
  err = clFinish(queue);

  /* Release the plan. */
  err = clfftDestroyPlan(&planHandle);

  /* Release clFFT library. */
  clfftTeardown();

  return output;
}

void fft_execute(const Array::Pointer & data)
{

  std::cout << "FFT execute" << std::endl;
  
  auto complex_data = fft_forward(data, nullptr);

  std::cout << "FFT forward is done" << std::endl;

  cle::print<float>(complex_data, "Complex data");


  std::cout << "Starting FFT backward" << std::endl;

  auto real_data = fft_backward(complex_data, nullptr);
  
  std::cout << "FFT backward is done" << std::endl;

  cle::print<float>(real_data, "Real data");

}

} // namespace cle::fft

#endif // __INCLUDE_FFT_HPP

