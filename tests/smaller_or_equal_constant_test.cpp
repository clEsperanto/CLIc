/**
 * Author: Stephane Rigaud - @strigaud 
 */

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <random>

#include "utils.h"
#include "tiffreader.h"
#include "tiffwriter.h"
#include "image.h"
#include "clbuffer.h"
#include "clgpu.h"

#include "clsmallerorequalconstant.h"




/**
 * Main test function
 *
 * Example of minipipeline operation.
 * Two kernel execution in a row, with push, create, and pull.
 *
 */
int main(int argc, char **argv)
{
    // Initialise random input and expected output.
    unsigned int width (5), height (5), depth (5);
    float* raw_data = new float[width*height*depth];
    float* res_data = new float[width*height*depth];
    float scalar = 50.0f;
    for (size_t i = 0; i < width*height*depth; i++)
    {
        if (i%2 == 0)
        {
            raw_data[i] = 100;
            res_data[i] = 0;
        }
        else
        {
            raw_data[i] = 50;
            res_data[i] = 1;
        }
    }

    // Initialise device, context, and CQ.
    clGPU gpu;
    gpu.Initialisation();

    // Push / Create input and output buffer
    Image<float> raw_image (raw_data, width, height, depth, "float");
    clBuffer gpuInput = gpu.Push<float>(raw_image);
    clBuffer gpuOutput = gpu.Create<float>(raw_image, "float");

    // Apply pipeline of kernels
    clSmallerOrEqualConstant addImageAndScalarKernel(gpu);
    addImageAndScalarKernel.Execute(gpuInput, gpuOutput, scalar);  

    // Pull output into container
    Image<float> out_image = gpu.Pull<float>(gpuOutput);    

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < width*height*depth; i++)
    {
        difference += std::abs(res_data[i] - out_image.GetData()[i]);
    }
    if (difference > std::numeric_limits<float>::epsilon())
    {
        std::cout << difference << " > " << std::numeric_limits<float>::epsilon() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}