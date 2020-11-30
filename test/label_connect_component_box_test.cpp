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
    unsigned int width (10), height (10), depth (2);
    int input_data[200] = {
                0,0,0,0,0,0,0,0,0,0,
                0,1,1,1,0,0,0,0,0,0,
                0,1,1,1,0,0,0,0,0,0,
                0,1,1,1,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,1,1,0,0,0,0,0,0,0,
                0,1,1,0,0,1,1,1,0,0,
                0,1,1,0,0,1,0,1,0,0,
                0,1,1,0,0,1,1,1,0,0,
                0,0,0,0,0,0,0,0,0,0,

                0,0,0,0,0,0,0,0,0,0,
                0,1,1,1,0,0,0,0,0,0,
                0,1,1,1,0,0,0,0,0,0,
                0,1,1,1,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,1,1,0,0,0,0,0,0,0,
                0,1,1,0,0,1,1,1,0,0,
                0,1,1,0,0,1,0,1,0,0,
                0,1,1,0,0,1,1,1,0,0,
                0,0,0,0,0,0,0,0,0,0
    };
    int valid_data[200] = {
                0,0,0,0,0,0,0,0,0,0,
                0,1,1,1,0,0,0,0,0,0,
                0,1,1,1,0,0,0,0,0,0,
                0,1,1,1,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,2,2,0,0,0,0,0,0,0,
                0,2,2,0,0,3,3,3,0,0,
                0,2,2,0,0,3,0,3,0,0,
                0,2,2,0,0,3,3,3,0,0,
                0,0,0,0,0,0,0,0,0,0,

                0,0,0,0,0,0,0,0,0,0,
                0,1,1,1,0,0,0,0,0,0,
                0,1,1,1,0,0,0,0,0,0,
                0,1,1,1,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,2,2,0,0,0,0,0,0,0,
                0,2,2,0,0,3,3,3,0,0,
                0,2,2,0,0,3,0,3,0,0,
                0,2,2,0,0,3,3,3,0,0,
                0,0,0,0,0,0,0,0,0,0
    };
    Image<int> input_img (input_data, width, height, depth, "int");

    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);

    // Initialise device memory and push from host to device
    cle::Buffer gpuInput = cle.Push<int>(input_img);
    cle::Buffer gpuOutput = cle.Create<int>(input_img);

    cle.LabelConnectComponentBox(gpuInput, gpuOutput);

    // pull device memory to host
    Image<int> output_img = cle.Pull<int>(gpuOutput);    

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
        std::cout << "Test failed, cumulated absolute difference " << difference << " > CPU epsilon (" << std::numeric_limits<float>::epsilon() << ")" << std::endl;
        // return EXIT_FAILURE;
    }

    // That's all folks!
    std::cout << "Test succeded!"<< std::endl;
    return EXIT_SUCCESS;
}