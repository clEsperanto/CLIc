
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
    unsigned int dims[3] = {width, height, depth};
    std::vector<float>  input_data {
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
    std::vector<float>  valid_data {
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

    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);

    // Initialise device memory and push from host to device
    cle::Buffer Buffer_A = cle.Push<float>(input_data, dims);
    cle::Buffer Buffer_B = cle.Create<float>(dims);

    // Call kernel
    cle.DifferenceOfGaussian(Buffer_A, Buffer_B, 2, 2, 2, 3, 3, 3);

    // pull device memory to host
    std::vector<float> output_data = cle.Pull<float>(Buffer_B);    

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < output_data.size(); i++)
    {
        difference += std::abs(valid_data[i] - std::round(output_data[i] * 1000000.0) / 1000000.0);
    }
    return difference > std::numeric_limits<float>::epsilon();
}