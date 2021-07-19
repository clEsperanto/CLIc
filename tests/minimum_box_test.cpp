
#include <random>

#include "CLE.h"

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    // Initialise random input and valid output.
    unsigned int width (3), height (3), depth (2);
    unsigned int dims[3] = {width, height, depth};
    std::vector<float> input_data {
                1, 1, 1,
                1, 0, 1,
                1, 1, 1,

                1, 1, 1,
                1, 1, 1,
                1, 1, 1
    };
    std::vector<float> valid_data {
                0, 0, 0,
                0, 0, 0,
                0, 0, 0,

                0, 0, 0,
                0, 0, 0,
                0, 0, 0
    };

    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);

    // Initialise device memory and push from host to device
    cle::Buffer Buffer_A = cle.Push<float>(input_data, dims);
    cle::Buffer Buffer_B = cle.Create<float>(dims);

    cle.MinimumBox(Buffer_A, Buffer_B, 1.5, 1.5, 1.5);

    // pull device memory to host
    std::vector<float> output_data = cle.Pull<float>(Buffer_B);    

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < output_data.size(); i++)
    {
        if (i % width == 0) {
            std::cout << std::endl;
        }
        std::cout << output_data[i] << " ";
        difference += std::abs(valid_data[i] - output_data[i]);
    }
    std::cout << std::endl;
    return difference > std::numeric_limits<float>::epsilon();
}