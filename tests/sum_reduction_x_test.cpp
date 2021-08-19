
#include <random>

#include "clesperanto.hpp"

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    // Initialise random input and valid output.
    int width (12), height (1), depth (1);
    int dims[3] = {width, height, depth};
    int block_size = 4;
    std::vector<float> input_data {0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0};
    std::vector<float> valid_data {2, 2, 1};

    // Initialise GPU information.
    cle::Clesperanto cle;
    
    // Initialise device memory and push from host
    cle::Buffer Buffer_A = cle.Push<float>(input_data, dims);
    int sum_dim = int(width / block_size);
    std::array<int, 3> new_dim = {sum_dim, 1, 1};
    cle::Buffer Buffer_B = cle.Create<float>(new_dim.data());

    // Call kernel
    cle.SumReductionX(Buffer_A, Buffer_B, block_size);  

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