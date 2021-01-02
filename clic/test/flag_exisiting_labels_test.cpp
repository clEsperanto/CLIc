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
    unsigned int width (4), height (4), depth (2);
    float input_data[32] = {
                1, 2, 0, 0,
                3, 0, 0, 0,
                0, 0, 0, 6,
                0, 0, 0, 7,

                1, 2, 0, 3,
                3, 3, 0, 1,
                0, 3, 0, 6,
                7, 0, 0, 7
    };

    float valid_data[8] = {
                1, 1, 1, 1, 0, 0, 1, 1
    };
    Image<float> input_img (input_data, width, height, depth, "float");

    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);

    // Initialise device memory and push from host to device
    cle::Buffer gpuInput = cle.Push<float>(input_img);
    std::array<unsigned int, 3> dimensions = {8, 1, 1}; //TODO: This should also work width flag depth=1, but it doesn't
    cle::Buffer gpuOutput = cle.Create<float>(dimensions.data(), "float");
    // cle::Buffer gpuOutput = cle.Create<float>(gpuInput, "float");

    // Call kernel
    cle.FlagExistingLabels(gpuInput, gpuOutput);

    // pull device memory to host
    Image<float> output_img = cle.Pull<float>(gpuOutput);    

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < 8; i++)
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