#ifndef __INCLUDE_FFT_HPP
#define __INCLUDE_FFT_HPP

#include "clFFT.h"

#include "backend.hpp"
#include "device.hpp"
#include "array.hpp"
#include "execution.hpp"

namespace cle::fft
{



clfftDim get_dim(Array::Pointer array)
{
  switch(array->dimension())
  {
    case 1:
      return CLFFT_1D;
    case 2:
      return CLFFT_2D;
    case 3:
      return CLFFT_3D;
    default:
      std::runtime_error("Invalid dimension");
  }
  return CLFFT_1D;
}


Array::Pointer create_complex_buffer(const Array::Pointer & data)
{
  cl_int   err;
  auto ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(data->device());

  // create a new buffer with twice the width for the imaginary part
  auto complex_array =   Array::create(data->width() * 2, data->height(), data->depth(), data->dimension(), data->dtype(), data->mtype(), data->device());

  std::array<size_t, 3> src_origin;  
  std::array<size_t, 3> dst_origin;
  std::array<size_t, 3> region = { 1 * data->itemSize(), data->height(), data->depth() };

  size_t src_row_pitch = data->width() * data->itemSize();
  size_t src_slice_pitch = src_row_pitch * data->height();

  size_t dst_row_pitch = complex_array->width() * complex_array->itemSize();
  size_t dst_slice_pitch = dst_row_pitch * complex_array->height();

  // loop over the width of the data and copy the real part to the buffer and set the imaginary part to 0
  for (size_t x = 0; x < data->width(); x++) {
    src_origin = { x * data->itemSize(), 0, 0 };
    dst_origin = { x * 2 * data->itemSize(), 0, 0 };

    clEnqueueCopyBufferRect(ocl_device->getCLCommandQueue(),
                            *static_cast<const cl_mem *>(*data->get()),
                            *static_cast<cl_mem *>(*complex_array->get()),
                            src_origin.data(),
                            dst_origin.data(),
                            region.data(),
                            src_row_pitch,
                            src_slice_pitch,
                            dst_row_pitch,
                            dst_slice_pitch,
                            0, NULL, NULL);
    }

  return complex_array;
}

void fft_execute(const Array::Pointer & data)
{
  // cast device to OpenCLDevice
  auto ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(data->device());
  auto ctx = ocl_device->getCLContext();
  auto queue = ocl_device->getCLCommandQueue();


  cl_int   err;
  cle::print<float>(data);

  auto complex_data = create_complex_buffer(data);

  cle::print<float>(complex_data);

  /* Setup clFFT. */
  clfftSetupData fftSetup;
  err = clfftInitSetupData(&fftSetup);
  err = clfftSetup(&fftSetup);

  /* FFT library realted declarations */
  clfftPlanHandle planHandle;
  clfftDim        dim = get_dim(data);
  size_t          clLengths[] = {data->width(), data->height()};

  /* Create a default plan for a complex FFT. */
  err = clfftCreateDefaultPlan(&planHandle, ctx, dim, clLengths);

  /* Set plan parameters. */
  err = clfftSetPlanPrecision(planHandle, CLFFT_SINGLE);
  err = clfftSetLayout(planHandle, CLFFT_COMPLEX_INTERLEAVED, CLFFT_COMPLEX_INTERLEAVED);
  err = clfftSetResultLocation(planHandle, CLFFT_INPLACE);

  /* Bake the plan. */
  err = clfftBakePlan(planHandle, 1, &queue, NULL, NULL);

  /* Execute the plan. */
  err = clfftEnqueueTransform(planHandle, CLFFT_FORWARD, 1, &queue, 0, NULL, NULL, static_cast<cl_mem *>(*complex_data->get()), NULL, NULL);

  /* Wait for calculations to be finished. */
  err = clFinish(queue);

  /* Release the plan. */
  err = clfftDestroyPlan(&planHandle);

  /* Release clFFT library. */
  clfftTeardown();

  cle::print<float>(complex_data);
}

} // namespace cle::fft

#endif // __INCLUDE_FFT_HPP
