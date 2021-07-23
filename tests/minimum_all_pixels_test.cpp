
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
    std::vector<float> valid_data (1);
    std::fill(input_data.begin(), input_data.end(), 1000); 
    input_data[50] = 10;
    valid_data[0] = 10;

    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);

    // Initialise device memory and push from host
    cle::Buffer Buffer_A = cle.Push<float>(input_data, dims);
    cle::Buffer Buffer_B = cle.Create<float>();

    // Call kernel
    cle.MinimumOfAllPixels(Buffer_A, Buffer_B);   

    // pull device memory to host
    std::vector<float> output_data = cle.Pull<float>(Buffer_B);    

    // Verify output
    float difference = std::abs(valid_data[0] - output_data[0]); 
    return difference > std::numeric_limits<float>::epsilon();
}
