
#include <random>

#include "CLE.h"

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    // Initialise random input and valid output.
    unsigned int width (12), height (1), depth (1);
    unsigned int dims[3] = {width, height, depth};

    std::vector<float> input_data{ 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0 };
    std::vector<float> valid_data{ 0, 1, 0, 2, 0, 0, 3, 4, 0, 0, 5, 0 };
    int max_label = width - 1;
    int blocksize = 4;
    
    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);

    // Initialise device memory and push from host to device
    cle::Buffer gpuFlagIndices = cle.Push<float>(input_data, dims);
    cle::Buffer gpuNewIndices = cle.Create<float>(dims);

    unsigned int block_value =  int((int(max_label) + 1) / blocksize) + 1;
    unsigned int block_dim[3] = {block_value, 1, 1};
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
        std::cout << valid_data[i] << " " << output_data[i] << std::endl;
        difference += std::abs(valid_data[i] - output_data[i]);
    }
    return difference > std::numeric_limits<float>::epsilon();
}