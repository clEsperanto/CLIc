
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
    std::array<int,3> dims = {width, height, depth};

    std::vector<float> input_data{ 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
    std::vector<float> valid_data{ 0.0f, 1.0f, 0.0f, 2.0f, 0.0f, 0.0f, 3.0f, 4.0f, 0.0f, 0.0f, 5.0f, 0.0f };
    int max_label = width - 1;
    int blocksize = 4;
    
    // Initialise GPU information.
    cle::Clesperanto cle;

    // Initialise device memory and push from host to device
    cle::Buffer gpuFlagIndices = cle.Push<float>(input_data, dims);
    cle::Buffer gpuNewIndices = cle.Create<float>(dims);

    int block_value =  int((int(max_label) + 1) / blocksize) + 1;
    std::array<int,3> block_dim = {block_value, 1, 1};
    cle::Buffer gpuBlockSums = cle.Create<float>(block_dim);

    // Call kernel
    cle.SumReductionX(gpuFlagIndices, gpuBlockSums, blocksize);
    cle.BlockEnumerate(gpuFlagIndices, gpuBlockSums, gpuNewIndices, blocksize);

    // pull device memory to host
    std::vector<float> output_data = cle.Pull<float>(gpuNewIndices);    

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < output_data.size(); i++)
    {
        difference += std::abs(valid_data[i] - output_data[i]);
    }
    return difference > std::numeric_limits<float>::epsilon();
}