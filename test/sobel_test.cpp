/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
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
    unsigned int width (5), height (5), depth (1);
    float* input_data = new float[width*height*depth];
    float* valid_data = new float[width*height*depth];
    for (size_t i = 0; i < width*height*depth; i++)
    {
        if (i % width == 0) {
            std::cout << std::endl;
        }

        input_data[i] = 0;
        if(i == 2*width+2)
        {
            input_data[i] = 1;
        }

        if(i < 6 || i > 19)
        {
            valid_data[i] = 0;
        }
        else if(i%5 == 0 || i%5 == 4)
        {
            valid_data[i] = 0;
        }
        else
        {
            valid_data[i] = sqrt(2);
        } 
        std::cout << input_data[i] << " ";
    }
    std::cout << std::endl;

    valid_data[2*width+2] = 0;
    valid_data[3*width+2] = 2;
    valid_data[1*width+2] = 2;
    valid_data[2*width+3] = 2;
    valid_data[2*width+1] = 2;

    Image<float> input_img (input_data, width, height, depth, "float");

    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);
    
    // Initialise device memory and push from host
    cle::Buffer gpuInput = cle.Push<float>(input_img);
    cle::Buffer gpuOutput = cle.Create<float>(input_img);

    // Call kernel
    cle.Sobel(gpuInput, gpuOutput);  

    // pull device memory to host
    Image<float> output_img = cle.Pull<float>(gpuOutput);  

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < width*height*depth; i++)
    {
        if (i % width == 0) {
            std::cout << std::endl;
        }
        std::cout << output_img.GetData()[i] << " ";
        difference += std::abs(valid_data[i] - output_img.GetData()[i]);
    }
    std::cout << std::endl;
    if (difference > std::numeric_limits<float>::epsilon())
    {
        std::cout << "Test failled, cumulated absolute difference " << difference << " > CPU epsilon (" << std::numeric_limits<float>::epsilon() << ")" << std::endl;
        return EXIT_FAILURE;
    }

    // That's all folks!
    std::cout << "Test succeded!"<< std::endl;
    return EXIT_SUCCESS;
}