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

#include "cleMaximumZProjection.h"

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    // Initialise random input and valid output.
    unsigned int width (10), height (10), depth (10);
    float* input_data = new float[width*height*depth];
    float* valid_data = new float[width*height*1];
    std::default_random_engine generator;
    std::normal_distribution<float> distribution(5.0,2.0);
    for (size_t d = 0; d < depth; d++)
    {
        for (size_t y = 0; y < height; y++)
        {
            for (size_t x = 0; x < width; x++)
            {
                int i = x + width*(y+height*d);
                if ( d == y )
                {
                    input_data[i] = 1000;
                }
                else
                {
                    input_data[i] = distribution(generator);
                }
            }
        }
    }
    for (size_t i = 0; i < width*height*1; i++)
    {
        valid_data[i] = 100;
    }

    // Initialise device, context, and CQ.
    cle::GPU gpu;
    gpu.Initialisation();

    // Push / Create input and output buffer
    Image<float> input_img (input_data, width, height, depth, "float");
    cle::Buffer gpuInput = gpu.Push<float>(input_img);
    std::array<unsigned int, 3> dimensions = {width, height, depth};
    dimensions.back() = 1;
    cle::Buffer gpuOutput = gpu.Create<float>(dimensions.data(), "float");

    // Apply pipeline of kernels
    cle::MaximumZProjection maximumProjection(gpu);
    maximumProjection.Execute(gpuInput, gpuOutput);   

    // Pull output into container
    Image<float> output_img = gpu.Pull<float>(gpuOutput);    

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < width*height*1; i++)
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