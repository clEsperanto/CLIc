
#include <random>

#include "clesperanto.hpp"

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    // Initialise random input and valid output.
    int width (4), height (4), depth (1);
    std::array<int,3> dims = {width, height, depth};
    std::vector<float> input_data {
                1, 2, 0, 0,
                3, 0, 0, 0,
                0, 0, 0, 6,
                0, 0, 0, 7
    };

    std::vector<float> valid_data {
                1, 1, 0, 0,
                1, 0, 0, 0,
                0, 0, 0, 6,
                0, 0, 0, 6
    };

    // Initialise GPU information.
    cle::Clesperanto cle;

    // Initialise device memory and push from host to device
    cle::Buffer Buffer_A = cle.Push<float>(input_data, dims);


    std::array<int, 3> dimensions = {1, 1, 2}; //TODO: This should also work width flag depth=1, but it doesn't
    cle::Buffer Buffer_B = cle.Create<float>(dimensions);
    cle::Buffer Buffer_C = cle.Create<float>(dims);

    // Call kernel
    cle.NonzeroMinimumBox(Buffer_A, Buffer_B, Buffer_C);

    // pull device memory to host
    std::vector<float> output_data = cle.Pull<float>(Buffer_C);    

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < output_data.size(); i++)
    {
        difference += std::abs(valid_data[i] - output_data[i]);
    }
    return difference > std::numeric_limits<float>::epsilon();
}