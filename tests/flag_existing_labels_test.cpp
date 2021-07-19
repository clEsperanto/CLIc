
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
    unsigned int dims[3] = {width, height, depth};
    std::vector<float> input_data {
                1, 2, 0, 0,
                3, 0, 0, 0,
                0, 0, 0, 6,
                0, 0, 0, 7,

                1, 2, 0, 3,
                3, 3, 0, 1,
                0, 3, 0, 6,
                7, 0, 0, 7
    };

    std::vector<float> valid_data {
                1, 1, 1, 1, 0, 0, 1, 1
    };

    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);

    // Initialise device memory and push from host to device
    cle::Buffer Buffer_A = cle.Push<float>(input_data, dims);
    std::array<unsigned int, 3> dimensions = {8, 1, 1}; //TODO: This should also work width flag depth=1, but it doesn't
    cle::Buffer Buffer_B = cle.Create<float>(dimensions.data());

    // Call kernel
    cle.FlagExistingLabels(Buffer_A, Buffer_B);

    // pull device memory to host
    std::vector<float> output_data = cle.Pull<float>(Buffer_B);    

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < output_data.size(); i++)
    {
        std::cout << valid_data[i] << " " << output_data[i] << std::endl;
        difference += std::abs(valid_data[i] - output_data[i]);
    }
    return difference > std::numeric_limits<float>::epsilon();
}