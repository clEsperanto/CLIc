
#include <random>

#include "clesperanto.hpp"

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    // Initialise random input and valid output.
    int width (10), height (10), depth (10);
    std::array<int,3> dims = {width, height, depth};
    std::vector<float> input_data (width*height*depth);
    std::vector<float> valid_data (1*height*depth);
    std::fill(input_data.begin(), input_data.end(), 1.0f);
    std::fill(valid_data.begin(), valid_data.end(), 10.0f);

    // Initialise GPU information.
    cle::Clesperanto cle;

    // Initialise device memory and push from host
    std::array<int, 3> new_dims = {height, depth, 1};
    cle::Buffer Buffer_A = cle.Push<float>(input_data, dims);
    cle::Buffer Buffer_B = cle.Create<float>(new_dims);

    // Call kernel
    cle.SumXProjection(Buffer_A, Buffer_B);   

    // pull device memory to host
    std::vector<float> output_data = cle.Pull<float>(Buffer_B);    

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < output_data.size(); i++)
    {
        difference += std::abs(valid_data[i] - output_data[i]);
    }
    return difference > std::numeric_limits<float>::epsilon();
}
