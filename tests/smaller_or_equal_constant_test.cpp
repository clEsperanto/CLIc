/**
 * Author: Stephane Rigaud - @strigaud 
 */

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <random>

#include "tiffreader.h"
#include "tiffwriter.h"
#include "image.h"

#include "cleSmallerOrEqualConstant.h"




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
    unsigned int width (10), height (10), depth (10);
    float* input_data = new float[width*height*depth];
    float* valid_data = new float[width*height*depth];
    float scalar = 50.0f;
    for (size_t i = 0; i < width*height*depth; i++)
    {
        if (i%2 == 0)
        {
            input_data[i] = 100;
            valid_data[i] = 0;
        }
        else
        {
            input_data[i] = 50;
            valid_data[i] = 1;
        }
    }

    // Initialise device, context, and CQ.
    cle::GPU gpu;
    gpu.Initialisation();

    // Push / Create input and output buffer
    Image<float> input_img (input_data, width, height, depth, "float");
    cle::Buffer gpuInput = gpu.Push<float>(input_img);
    cle::Buffer gpuOutput = gpu.Create<float>(input_img, "float");

    // Apply pipeline of kernels
    cle::SmallerOrEqualConstant addImageAndScalarKernel(gpu);
    addImageAndScalarKernel.Execute(gpuInput, gpuOutput, scalar);  

    // Pull output into container
    Image<float> output_img = gpu.Pull<float>(gpuOutput);    

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < width*height*depth; i++)
    {
        difference += std::abs(valid_data[i] - output_img.GetData()[i]);
    }
    if (difference > std::numeric_limits<float>::epsilon())
    {
        std::cout << "Test failled, cumulated absolute difference " << difference << " > CPU epsilon (" << std::numeric_limits<float>::epsilon() << ")" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Test succeded!"<< std::endl;
    return EXIT_SUCCESS;
}