
#include <random>

#include "clesperanto.hpp"

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    // Initialise random input and valid output.
    int width (5), height (1), depth (1);
    std::array<int,3> dims = {width, height, depth};
    std::vector<float> input_data (width*height*depth);
    std::vector<float> valid_data (width*height*depth);
    for (size_t i = 0; i < input_data.size(); i++)
    {
            input_data[i] = static_cast<float>(i);
            valid_data[i] = static_cast<float>(i);
    }
    valid_data[2] = 10.0f;

    // Initialise GPU information.
    cle::Clesperanto cle;

    // Initialise device memory and push from host to device
    cle::Buffer Buffer_A = cle.Push<float>(input_data, dims);

    // Call kernel
    cle.SetColumn(Buffer_A, 2, 10);

    // pull device memory to host
    std::vector<float> output_data = cle.Pull<float>(Buffer_A);    

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < output_data.size(); i++)
    {
        difference += std::abs(valid_data[i] - output_data[i]);
    }
    return difference > std::numeric_limits<float>::epsilon();
}