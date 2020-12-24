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
    unsigned int width (4), height (3), depth (2);
    float input_data1[24] = {
                1, 2, 3, 4,
                5, 6, 7, 8,
                9, 0, 1, 2,

                3, 4, 5, 6,
                7, 8, 9, 0,
                1, 2, 3, 4
    };
    float* input_data2 = new float[width*height*depth];
    // will be set to 5 everywhere, further down

    float valid_data[24] = {
                0, 0, 0, 0,
                1, 0, 0, 0,
                0, 0, 0, 0,

                0, 0, 1, 0,
                0, 0, 0, 0,
                0, 0, 0, 0
    };
    Image<float> input_img1 (input_data1, width, height, depth, "float");
    Image<float> input_img2 (input_data2, width, height, depth, "float");

    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);

    // Initialise device memory and push from host to device
    cle::Buffer gpuInput1 = cle.Push<float>(input_img1);
    cle::Buffer gpuInput2 = cle.Push<float>(input_img2);

    cle.Set(gpuInput2, 5);

    cle::Buffer gpuOutput = cle.Create<float>(gpuInput1, "float");

    // Call kernel
    cle.Equal(gpuInput1, gpuInput2, gpuOutput);

    // pull device memory to host
    Image<float> output_img = cle.Pull<float>(gpuOutput);    

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < width*height*depth; i++)
    {
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