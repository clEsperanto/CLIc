
#include <iostream>
#include "clesperanto.hpp"

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    int width (10), height (10), depth (10);
    int dims[3] = {width, height, depth};
    std::vector<float> input_data (width*height*depth);
    std::vector<float> valid_data (width*height*depth);
    float scalar = 100.0f;
    std::fill(input_data.begin(), input_data.end(), 5.0f);
    std::fill(valid_data.begin(), valid_data.end(), 5.0f + scalar);

    cle::Clesperanto cle;

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