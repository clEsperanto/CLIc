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
    Image<float> input_img (input_data, width, height, depth, "float");

    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);

    // Initialise device memory and push from host to device
    cle::Buffer gpuInput = cle.Push<float>(input_img);
    cle::Buffer gpuOutput = cle.Create<float>(input_img, "float");

    // Call kernel
    cle.SmallerOrEqualConstant(gpuInput, gpuOutput, scalar);  

    // pull device memory to host
    Image<float> output_img = cle.Pull<float>(gpuOutput);    

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

    // That's all folks!
    std::cout << "Test succeded!"<< std::endl;
    return EXIT_SUCCESS;
}