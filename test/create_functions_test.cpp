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

    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);
    
    // Initialise device memory and push from host
    cle::Buffer gpuOutput;
    try
    {
        gpuOutput = cle.Create<float>(input_img);
    }
    catch(cl_int clError)
    {
        std::cerr << getOpenCLErrorString(clError) << std::endl;
        return EXIT_FAILURE;
    }
    
    // That's all folks!
    std::cout << "Test succeded!"<< std::endl;
    return EXIT_SUCCESS;
}