
#include <iostream>
#include "CLE.hpp"

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    unsigned int width (10), height (10), depth (10);
    unsigned int dims[3] = {width, height, depth};
    std::vector<float> input_data (width*height*depth);
    std::vector<float> valid_data (width*height*depth);
    float scalar = 100;
    std::fill(input_data.begin(), input_data.end(), 5);
    std::fill(valid_data.begin(), valid_data.end(), 5 + scalar);

    cle::GPU gpu;
    cle::CLE cle(gpu);

    cle::Buffer Buffer_A = cle.Push<float>(input_data, dims);
    cle::Buffer Buffer_B = cle.Create<float>(dims);

    cle.AddImageAndScalar(Buffer_A, Buffer_B, scalar);

    std::vector<float> output_data = cle.Pull<float>(Buffer_B);

    float difference = 0;
    for (size_t i = 0; i < output_data.size(); i++)
    {
        difference += std::abs(valid_data[i] - output_data[i]);
    }
    return difference > std::numeric_limits<float>::epsilon();
}