/**
 * Author:  Robert Haase - @haesleinhuepf
 *          Stephane Rigaud - @strigaud 
 */

#include <random>

#include "CLE.h"

/**
 * Main test function
 *
 */
int main(int argc, char** argv)
{
    // Initialise random input and valid output.
    unsigned int width(5), height(5);
    float* input_data = new float[width * height];
    float* valid_data = new float[width * height];
    for (size_t i = 0; i < width * height; i++)
    {
        if (i == 12) // center pixel
        {
            input_data[i] = 5;
        }
        else
        {
            input_data[i] = 0;
        }
        if ((i == 7) || (i > 10 && i < 14) || (i == 17)) {
            valid_data[i] = 1;
        } else {
            valid_data[i] = 0;
        }
    }

    Image<float> input_img (input_data, width, height, 1, "float");

    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);
    
    // Initialise device memory and push from host
    cle::Buffer gpuInput = cle.Push<float>(input_img);
    cle::Buffer gpuOutput = cle.Create<float>(input_img);

    // Call kernel
    cle.Mean2DSphere(gpuInput, gpuOutput, 1, 1);  

    // pull device memory to host
    Image<float> output_img = cle.Pull<float>(gpuOutput);    

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < width*height; i++)
    {
        if (i % width == 0) {
            std::cout << std::endl;
        }
        std::cout << output_img.GetData()[i] << " ";
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