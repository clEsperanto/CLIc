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
    unsigned int width (12), height (1), depth (1);
    int block_size = 4;
    float input_data[12] = {0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0};
    float valid_data[3] = {2, 2, 1};
    Image<float> input_img (input_data, width, height, depth, "float");

    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);
    
    // Initialise device memory and push from host
    cle::Buffer gpuInput = cle.Push<float>(input_img);
    unsigned int sum_dim = int(width / block_size);
    std::array<unsigned int, 3> dimensions = {sum_dim, 1, 1};
    cle::Buffer gpuOutput = cle.Create<float>(dimensions.data(), "float");

    // Call kernel
    cle.SumReductionX(gpuInput, gpuOutput, block_size);  

    // pull device memory to host
    Image<float> output_img = cle.Pull<float>(gpuOutput);    

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < sum_dim; i++)
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