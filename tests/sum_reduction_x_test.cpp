
#include <random>

#include "CLE.hpp"

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    // Initialise random input and valid output.
    unsigned int width (12), height (1), depth (1);
    unsigned int dims[3] = {width, height, depth};
    int block_size = 4;
    std::vector<float> input_data {0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0};
    std::vector<float> valid_data {2, 2, 1};

    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);
    
    // Initialise device memory and push from host
    cle::Buffer Buffer_A = cle.Push<float>(input_data, dims);
    unsigned int sum_dim = int(width / block_size);
    std::array<unsigned int, 3> dimensions = {sum_dim, 1, 1};
    cle::Buffer Buffer_B = cle.Create<float>(dimensions.data());

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