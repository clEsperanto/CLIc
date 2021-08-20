
#include <random>

#include "clesperanto.hpp"

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    // Initialise random input and valid output.
    int width (5), height (5), depth (1);
    int dims[3] = {width, height, depth};
    std::vector<float> input_data {
                0, 0, 0, 0, 0,
                0, 1, 2, 3, 0,
                0, 2, 3, 4, 0,
                0, 4, 4, 5, 0,
                0, 0, 0, 0, 0
    };

    std::vector<float> valid_data {
                0, 0, 0, 0, 0,
                0, 9, 8, 7, 0,
                0, 8, 7, 6, 0,
                0, 6, 6, 5, 0,
                0, 0, 0, 0, 0
    }; 
    std::vector<float> reference_data {
                0, 9, 8, 7, 6, 5
    };

    // Initialise GPU information.
    cle::Clesperanto cle;

    // Initialise device memory and push from host to device
    cle::Buffer Buffer_A = cle.Push<float>(input_data, dims);
    int Buffer_B_dims[3] = {6, 1, 1};
    cle::Buffer Buffer_B = cle.Push<float>(reference_data, Buffer_B_dims);
    cle::Buffer Buffer_C = cle.Create<float>(dims);

    // Call kernel
    cle.ReplaceIntensities(Buffer_A, Buffer_B, Buffer_C);

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