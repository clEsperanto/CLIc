
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
    std::vector<float> valid_data (width*height*depth);
    for (size_t i = 0; i < width*height*depth; i++)
    {
        if (width%2 == 0)
        {
            input_data[i] = 10.0f;
            valid_data[i] = 10.0f;
        }
        else
        {
            input_data[i] = 1.0f;
            valid_data[i] = 1.0f;
        }
    }

    // Initialise GPU information.
    cle::Clesperanto cle;
    
    cle::Buffer Buffer_A = cle.Push<float>(input_data, dims);
    cle::Buffer Buffer_B = cle.Create<float>(dims);

    // Call kernel
    cle.Copy(Buffer_A, Buffer_B);  

    // pull device memory to host
    std::vector<float> ouput_data = cle.Pull<float>(Buffer_B);

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < ouput_data.size(); i++)
    {
        difference += std::abs(valid_data[i] - ouput_data[i]);
    }
    return difference > std::numeric_limits<float>::epsilon();
}