
#include <random>

#include "clesperanto.hpp"

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    // Initialise random input and valid output.
    int width (3), height (3), depth (3);
    int dims[3] = {width, height, depth};
    std::vector<float>  input_data {
                0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f,

                0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f,

                0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f
    };
    std::vector<float>  valid_data {
                0.003464f, 0.004077f, 0.003464f,
                0.004077f, 0.004780f, 0.004077f,
                0.003464f, 0.004077f, 0.003464f,

                0.004077f, 0.004780f, 0.004077f,
                0.004780f, 0.005585f, 0.004780f,
                0.004077f, 0.004780f, 0.004077f,

                0.003464f, 0.004077f, 0.003464f,
                0.004077f, 0.004780f, 0.004077f,
                0.003464f, 0.004077f, 0.003464f  
    };

    // Initialise GPU information.
    cle::Clesperanto cle;

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
        difference += std::abs(valid_data[i] - std::round(output_data[i] * 1000000.0f) / 1000000.0f);
    }
    return difference > std::numeric_limits<float>::epsilon();
}