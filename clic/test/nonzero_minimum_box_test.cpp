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
    unsigned int width (4), height (4), depth (1);
    float input_data1[24] = {
                1, 2, 0, 0,
                3, 0, 0, 0,
                0, 0, 0, 6,
                0, 0, 0, 7
    };

    float valid_data[16] = {
                1, 1, 0, 0,
                1, 0, 0, 0,
                0, 0, 0, 6,
                0, 0, 0, 6
    };
    Image<float> input_img1 (input_data1, width, height, depth, "float");

    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);

    // Initialise device memory and push from host to device
    cle::Buffer gpuInput1 = cle.Push<float>(input_img1);


    std::array<unsigned int, 3> dimensions = {1, 1, 2}; //TODO: This should also work width flag depth=1, but it doesn't
    cle::Buffer gpuFlag = cle.Create<float>(dimensions.data(), "float");

    cle::Buffer gpuOutput = cle.Create<float>(gpuInput1, "float");

    // Call kernel
    cle.NonzeroMinimumBox(gpuInput1, gpuFlag, gpuOutput);

    // pull device memory to host
    Image<float> output_img = cle.Pull<float>(gpuOutput);    

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < width*height*depth; i++)
    {
        std::cout << valid_data[i] << " " << output_img.GetData()[i] << std::endl;
        difference += std::abs(valid_data[i] - output_img.GetData()[i]);
    }
    if (difference > std::numeric_limits<float>::epsilon())
    {
        std::cout << "Test failed, cumulated absolute difference " << difference << " > CPU epsilon (" << std::numeric_limits<float>::epsilon() << ")" << std::endl;
        return EXIT_FAILURE;
    }

    // That's all folks!
    std::cout << "Test succeded!"<< std::endl;
    return EXIT_SUCCESS;
}