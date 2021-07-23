
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
    unsigned int width (10), height (10), depth (10);
    unsigned int dims[3] = {width, height, depth};
    std::vector<float> input_data (width*height*depth);
    std::vector<float> valid_data (width*height*depth);
    for (size_t i = 0; i < width*height*depth; i++)
    {
        if (width%2 == 0)
        {
            input_data[i] = 10;
            valid_data[i] = 10;
        }
        else
        {
            input_data[i] = 1;
            valid_data[i] = 1;
        }
    }

    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);
    
    cle::Buffer Buffer_A = cle.Push<float>(input_data, dims);
    cle::Buffer Buffer_B = cle.Create<float>(dims);

    // Call kernel
    cle.Copy(Buffer_A, Buffer_B);  

    // pull device memory to host
    std::vector<float> ouput_data = cle.Pull<float>(Buffer_B);

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < ouput_data.size(); i++)
    {
        difference += std::abs(valid_data[i] - ouput_data[i]);
    }
    return difference > std::numeric_limits<float>::epsilon();
}