
#include <random>
#include <iostream>

#include "CLE.h"

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    // Initialise random input and valid output.
    unsigned int width (3), height (3), depth (3);
    std::vector<float> input_data (width*height*depth);
    std::fill(input_data.begin(), input_data.end(), 0);
    input_data[13] = 1;
    std::vector<float> valid_data = {
                0.0141675, 0.0233582, 0.0141675,
                0.0233582, 0.0385112, 0.0233582,
                0.0141675, 0.0233582, 0.0141675,

                0.0233582, 0.0385112, 0.0233582,
                0.0385112, 0.0634942, 0.0385112,
                0.0233582, 0.0385112, 0.0233582,

                0.0141675, 0.0233582, 0.0141675,
                0.0233582, 0.0385112, 0.0233582,
                0.0141675, 0.0233582, 0.0141675
    };


    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);

    unsigned int dim[3] = {width, height, depth};
    cle::Buffer Buffer_A = cle.Push<float>(input_data, dim);
    cle::Buffer Buffer_B = cle.Create<float>(dim);

    cle.GaussianBlur(Buffer_A, Buffer_B, 1, 1, 1);

    // pull device memory to host
    std::vector<float> ouput_data = cle.Pull<float>(Buffer_B);  

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < ouput_data.size(); i++)
    {
        difference += std::abs(valid_data[i] - std::round(ouput_data[i] * 10000000.0) / 10000000.0);
    }
    return difference > std::numeric_limits<float>::epsilon();
}