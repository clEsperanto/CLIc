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
    float input_data[27] = {
                0, 0, 0,
                0, 0, 0,
                0, 0, 0,

                0, 0, 0,
                0, 1, 0,
                0, 0, 0,

                0, 0, 0,
                0, 0, 0,
                0, 0, 0    
    };
    float valid_data[27] = {
                0.003464, 0.004077, 0.003464,
                0.004077, 0.004780, 0.004077,
                0.003464, 0.004077, 0.003464,

                0.004077, 0.004780, 0.004077,
                0.004780, 0.005585, 0.004780,
                0.004077, 0.004780, 0.004077,

                0.003464, 0.004077, 0.003464,
                0.004077, 0.004780, 0.004077,
                0.003464, 0.004077, 0.003464  
    };
    Image<float> input_img (input_data, width, height, depth, "float");

    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);

    // Initialise device memory and push from host to device
    cle::Buffer gpuInput = cle.Push<float>(input_img);
    cle::Buffer gpuOutput = cle.Create<float>(gpuInput, "float");

    // Call kernel
    cle.DifferenceOfGaussian3D(gpuInput, gpuOutput, 2, 2, 2, 3, 3, 3);

    // pull device memory to host
    Image<float> output_img = cle.Pull<float>(gpuOutput);    

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < width*height*depth; i++)
    {
        if(i%width==0)
        {
            std::cout << std::endl;
        }
        std::cout << output_img.GetData()[i] << " ";
        difference += std::abs(valid_data[i] - std::round(output_img.GetData()[i] * 1000000.0) / 1000000.0);
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