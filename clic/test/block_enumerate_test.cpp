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
    float input_data[12] = {
                0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0
    };
    float valid_data[12] = {
                0, 1, 0, 2, 0, 0, 3, 4, 0, 0, 5, 0
    };
    int max_label = width - 1;
    int blocksize = 4;
    
    Image<float> input_img (input_data, width, height, depth, "float");

    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);

    // Initialise device memory and push from host to device
    cle::Buffer gpuFlagIndices = cle.Push<float>(input_img);
    cle::Buffer gpuNewIndices = cle.Create<float>(gpuFlagIndices, "float");

    unsigned int block_value =  int((int(max_label) + 1) / blocksize) + 1;
    unsigned int block_dim[3] = {block_value, 1, 1};
    cle::Buffer gpuBlockSums = cle.Create<float>(block_dim, "float");

    // Call kernel
    cle.SumReductionX(gpuFlagIndices, gpuBlockSums, blocksize);
    cle.BlockEnumerate(gpuFlagIndices, gpuBlockSums, gpuNewIndices, blocksize);

    // pull device memory to host
    Image<float> output_img = cle.Pull<float>(gpuNewIndices);    

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