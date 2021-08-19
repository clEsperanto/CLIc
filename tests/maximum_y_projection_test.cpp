
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
    int dim[3] = {width, height, depth};
    std::vector<float> input_data ({
        10, 1, 5, 
         1,10, 6, 
         5, 8,10,

         1, 1,10, 
         1,10, 6, 
        10, 8, 4,

        10, 1,10,
         1, 1, 6,
         5,10, 8, 
    });
    std::vector<float> valid_data (width*1*depth);
    std::fill(valid_data.begin(), valid_data.end(), 10);


    // Initialise GPU information.
    cle::Clesperanto cle;

    cle::Buffer Buffer_A = cle.Push<float>(input_data, dim);
    std::array<int,3> new_dim = {width, depth, 1};
    cle::Buffer Buffer_B = cle.Create<float>(new_dim.data());

    // Call kernel
    cle.MaximumYProjection(Buffer_A, Buffer_B);   

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
