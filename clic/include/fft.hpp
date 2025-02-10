#ifndef __INCLUDE_FFT_HPP
#define __INCLUDE_FFT_HPP

#include "clFFT.h"

#include "backend.hpp"
#include "device.hpp"

namespace cle::fft
{

void fft_demo(const Device::Pointer & device)
{
	// cast device to OpenCLDevice
	auto ocl_device = std::dynamic_pointer_cast<OpenCLDevice>(device);

    cl_int err;
    cl_mem bufX;
	float *X;
    cl_event event = NULL;
    int ret = 0;
	size_t N = 16;

	// /* FFT library realted declarations */
	clfftPlanHandle planHandle;
	clfftDim dim = CLFFT_1D;
	size_t clLengths[1] = {N};

    auto ctx = ocl_device->getCLContext();
    auto queue = ocl_device->getCLCommandQueue();

	/* Setup clFFT. */
	clfftSetupData fftSetup;
	err = clfftInitSetupData(&fftSetup);
	err = clfftSetup(&fftSetup);

	/* Allocate host & initialize data. */
	/* Only allocation shown for simplicity. */
	X = (float *)malloc(N * 2 * sizeof(*X));

    /* Prepare OpenCL memory objects and place data inside them. */
    bufX = clCreateBuffer( ctx, CL_MEM_READ_WRITE, N * 2 * sizeof(*X), NULL, &err );

    err = clEnqueueWriteBuffer( queue, bufX, CL_TRUE, 0,
	N * 2 * sizeof( *X ), X, 0, NULL, NULL );

	/* Create a default plan for a complex FFT. */
	err = clfftCreateDefaultPlan(&planHandle, ctx, dim, clLengths);

	/* Set plan parameters. */
	err = clfftSetPlanPrecision(planHandle, CLFFT_SINGLE);
	err = clfftSetLayout(planHandle, CLFFT_COMPLEX_INTERLEAVED, CLFFT_COMPLEX_INTERLEAVED);
	err = clfftSetResultLocation(planHandle, CLFFT_INPLACE);

    /* Bake the plan. */
	err = clfftBakePlan(planHandle, 1, &queue, NULL, NULL);

	/* Execute the plan. */
	err = clfftEnqueueTransform(planHandle, CLFFT_FORWARD, 1, &queue, 0, NULL, NULL, &bufX, NULL, NULL);

	/* Wait for calculations to be finished. */
	err = clFinish(queue);

	/* Fetch results of calculations. */
	err = clEnqueueReadBuffer( queue, bufX, CL_TRUE, 0, N * 2 * sizeof( *X ), X, 0, NULL, NULL );

    /* Release OpenCL memory objects. */
    clReleaseMemObject( bufX );

	free(X);

	/* Release the plan. */
	err = clfftDestroyPlan( &planHandle );

    /* Release clFFT library. */
    clfftTeardown( );

}


    
} // namespace cle::fft

#endif // __INCLUDE_FFT_HPP