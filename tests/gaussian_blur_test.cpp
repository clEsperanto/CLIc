
#include <random>
#include <iostream>

#include "clesperanto.hpp"

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    // Initialise random input and valid output.
    int width (3), height (3), depth (3);
    std::array<int,3> dims = {width, height, depth};
    std::vector<float> input_data (width*height*depth);
    std::fill(input_data.begin(), input_data.end(), 0);
    input_data[13] = 1;
    std::vector<float> valid_data = {
                0.0141675f, 0.0233582f, 0.0141675f,
                0.0233582f, 0.0385112f, 0.0233582f,
                0.0141675f, 0.0233582f, 0.0141675f,

                0.0233582f, 0.0385112f, 0.0233582f,
                0.0385112f, 0.0634942f, 0.0385112f,
                0.0233582f, 0.0385112f, 0.0233582f,

                0.0141675f, 0.0233582f, 0.0141675f,
                0.0233582f, 0.0385112f, 0.0233582f,
                0.0141675f, 0.0233582f, 0.0141675f
    };


    // Initialise GPU information.
    cle::Clesperanto cle;

    cle::Buffer Buffer_A = cle.Push<float>(input_data, dims);
    cle::Buffer Buffer_B = cle.Create<float>(dims);

    cle.GaussianBlur(Buffer_A, Buffer_B, 1, 1, 1);

    // pull device memory to host
    std::vector<float> ouput_data = cle.Pull<float>(Buffer_B);  

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < ouput_data.size(); i++)
    {
        difference += std::abs(valid_data[i] - std::round(ouput_data[i] * 10000000.0f) / 10000000.0f);
    }
    return difference > std::numeric_limits<float>::epsilon();
}