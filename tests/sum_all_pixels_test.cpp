
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
    int dims[3] = {width, height, depth};
    std::vector<float> input_data (width*height*depth);
    std::fill(input_data.begin(), input_data.end(), 1.0f);
    std::vector<float> valid_data(1);
    valid_data[0] = 1000;

    // Initialise GPU information.
    cle::Clesperanto cle;

    // Initialise device memory and push from host
    cle::Buffer Buffer_A = cle.Push<float>(input_data, dims);
    cle::Buffer Buffer_B = cle.Create<float>();

    // Call kernel
    cle.SumOfAllPixels(Buffer_A, Buffer_B);   

    // pull device memory to host
    std::vector<float> output_data = cle.Pull<float>(Buffer_B);    

    // Verify output
    float difference = std::abs(valid_data[0] - output_data[0]); 
    return difference > std::numeric_limits<float>::epsilon();
}
