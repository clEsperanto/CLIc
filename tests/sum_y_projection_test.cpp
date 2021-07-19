
#include <random>

#include "CLE.h"

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    // Initialise random input and valid output.
    unsigned int width (10), height (10), depth (10);
    unsigned int dims[3] = {width, height, depth};
    std::vector<float> input_data (width*height*depth);
    std::vector<float> valid_data (width*1*depth);
    std::fill(input_data.begin(), input_data.end(), 1);
    std::fill(valid_data.begin(), valid_data.end(), 10);

    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);

    // Initialise device memory and push from host
    std::array<unsigned int, 3> dimensions = {width, height, depth};
    dimensions.back() = 1;
    cle::Buffer Buffer_A = cle.Push<float>(input_data, dims);
    cle::Buffer Buffer_B = cle.Create<float>(dimensions.data());

    // Call kernel
    cle.SumYProjection(Buffer_A, Buffer_B);   

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
