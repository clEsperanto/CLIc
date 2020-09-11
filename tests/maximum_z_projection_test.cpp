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
#include "clmaximumzprojection.h"

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
    std::default_random_engine generator;
    std::normal_distribution<float> distribution(5.0,2.0);
    unsigned int width (5), height (5), depth (5);
    float* raw_data = new float[width*height*depth];
    for (size_t d = 0; d < depth; d++)
    {
        for (size_t y = 0; y < height; y++)
        {
            for (size_t x = 0; x < width; x++)
            {
                int i = x + width*(y+height*d);
                if ( d == y )
                {
                    raw_data[i] = 100;
                }
                else
                {
                    raw_data[i] = distribution(generator);
                }
            }
        }
    }
    float* res_data = new float[width*height*1];
    for (size_t i = 0; i < width*height*1; i++)
    {
        res_data[i] = 100;
    }

    // Initialise device, context, and CQ.
    clGPU gpu;
    gpu.Initialisation();

    // Push / Create input and output buffer
    Image<float> raw_image (raw_data, width, height, depth, "float");
    clBuffer gpuInput = gpu.Push<float>(raw_image);
    std::array<unsigned int, 3> dimensions = {width, height, depth};
    dimensions.back() = 1;
    clBuffer gpuOutput = gpu.Create<float>(dimensions.data(), "float");

    // Apply pipeline of kernels
    clMaximumZProjection maximumProjection(gpu);
    maximumProjection.Execute(gpuInput, gpuOutput);   

    // Pull output into container
    Image<float> out_image = gpu.Pull<float>(gpuOutput);    

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < width*height*1; i++)
    {
        difference += std::abs(res_data[i] - out_image.GetData()[i]);
    }
    if (difference > std::numeric_limits<float>::epsilon())
    {
        std::cout << difference << " > " << std::numeric_limits<float>::epsilon() << " , the test fail!" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}