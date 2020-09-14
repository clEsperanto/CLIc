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

#include "cleAddImageAndScalar.h"

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    // Initialise input and validation, and fill with values.
    unsigned int width (10), height (10), depth (10);
    float* input_data = new float[width*height*depth];
    float* valid_data = new float[width*height*depth];
    float scalar = 100.0f;
    std::default_random_engine generator;
    std::normal_distribution<float> distribution(5.0,2.0);
    for (size_t i = 0; i < width*height*depth; i++)
    {
        input_data[i] = distribution(generator);
        valid_data[i] = input_data[i] + scalar;
    }

    // Initialise device, context, and CQ.
    cle::GPU gpu;
    gpu.Initialisation();

    // Push / Create input and output buffer
    Image<float> input_img (input_data, width, height, depth, "float");
    cle::Buffer gpuInput = gpu.Push<float>(input_img);
    cle::Buffer gpuOutput = gpu.Create<float>(input_img, "float");

    // Apply pipeline of kernels
    cle::AddImageAndScalar addImageAndScalarKernel(gpu);
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