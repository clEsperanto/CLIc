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
    unsigned int width (3), height (3), depth (3);
    unsigned int arrSize = width * height * depth;
    float input_data[arrSize];
    for (size_t i = 0; i < arrSize; i++)
    {
        input_data[i] = 0;
    }
    input_data[13] = 1;
    float valid_data[arrSize] = {
                0, 0.03851, 0,
                0.03851, 0.03851, 0.03851,
                0, 0.03851, 0,

                0, 0.03851, 0,
                0.03851, 0.06349, 0.03851,
                0, 0.03851, 0,

                0, 0.03851, 0,
                0.03851, 0.03851, 0.03851,
                0, 0.03851, 0
    };
    Image<float> input_img (input_data, width, height, depth, "float");

    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);

    // Initialise device memory and push from host to device
    cle::Buffer gpuInput = cle.Push<float>(input_img);
    cle::Buffer gpuOutput = cle.Create<float>(gpuInput);

    cle.GaussianBlur(gpuInput, gpuOutput, 0.4, 0.4, 0.4);

    // pull device memory to host
    Image<float> output_img = cle.Pull<float>(gpuOutput);    

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < arrSize; i++)
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
        std::cout << "Test failed, cumulated absolute difference " << difference << " > CPU epsilon (" << std::numeric_limits<float>::epsilon() << ")" << std::endl;
        return EXIT_FAILURE;
    }

    // That's all folks!
    std::cout << "Test succeded!"<< std::endl;
    return EXIT_SUCCESS;
}