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
    float input_data[25] = {
                0, 0, 0, 0, 0,
                0, 1, 2, 3, 0,
                0, 2, 3, 4, 0,
                0, 4, 4, 5, 0,
                0, 0, 0, 0, 0
    };

    float valid_data[25] = {
                0, 0, 0, 0, 0,
                0, 9, 8, 7, 0,
                0, 8, 7, 6, 0,
                0, 6, 6, 5, 0,
                0, 0, 0, 0, 0
    }; 
    float reference_data[6] = {
                0, 9, 8, 7, 6, 5
    };
    Image<float> input_img (input_data, width, height, depth, "float");
    Image<float> refence_img (reference_data, 6, 1, 1, "float");

    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);

    // Initialise device memory and push from host to device
    cle::Buffer gpuInput = cle.Push<float>(input_img);
    cle::Buffer gpuReference = cle.Push<float>(refence_img);
    cle::Buffer gpuOutput = cle.Create<float>(gpuInput, "float");

    // Call kernel
    cle.ReplaceIntensities(gpuInput, gpuReference, gpuOutput);

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