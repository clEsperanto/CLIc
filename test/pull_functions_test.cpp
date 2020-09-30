/**
 * Author: Stephane Rigaud - @strigaud 
 */

#include <random>

#include "CLE.h"

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    // Initialise random input and valid output.
    unsigned int width (10), height (10), depth (10);
    float* input_data = new float[width*height*depth];
    
    std::default_random_engine generator;
    std::normal_distribution<float> distribution(5.0,2.0);
    for (size_t i = 0; i < width*height*depth; i++)
    {
        input_data[i] = distribution(generator);
    }
    Image<float> input_img (input_data, width, height, depth, "float");
    Image<float> output_img;

    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);
    
    // Initialise device memory and push from host
    cle::Buffer gpuInput, gpuOutput;
    try
    {
        gpuInput = cle.Push<float>(input_img);
    }
    catch(cl_int clError)
    {
        std::cerr << getOpenCLErrorString(clError) << std::endl;
        return EXIT_FAILURE;
    }

    try
    {
        output_img = cle.Pull<float>(gpuInput);
    }
    catch(cl_int clError)
    {
        std::cerr << getOpenCLErrorString(clError) << std::endl;
        return EXIT_FAILURE;
    }

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < width*height*depth; i++)
    {
        difference += std::abs(input_data[i] - output_img.GetData()[i]);
    }
    if (difference > std::numeric_limits<float>::epsilon())
    {
        std::cout << "Test failled, cumulated absolute difference " << difference << " > CPU epsilon (" << std::numeric_limits<float>::epsilon() << ")" << std::endl;
        return EXIT_FAILURE;
    }
    
    // That's all folks!
    std::cout << "Test succeded!"<< std::endl;
    return EXIT_SUCCESS;
}