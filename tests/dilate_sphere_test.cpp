
#include <random>

#include "CLE.hpp"

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
    std::vector<float> valid_data (width*height*depth);
    std::fill(input_data.begin(), input_data.end(), 0);
    std::fill(valid_data.begin(), valid_data.end(), 0);

    int central_idx = (width/2) + (height/2)*height + (depth/2) * height * width;
    input_data[central_idx] = 1;
    valid_data[central_idx] = 1;
    valid_data[central_idx+1] = 1;
    valid_data[central_idx-1] = 1;
    valid_data[central_idx+height] = 1;
    valid_data[central_idx-height] = 1;
    valid_data[central_idx+(height * width)] = 1;
    valid_data[central_idx-(height * width)] = 1;

    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);
    
    // Initialise device memory and push from host
    cle::Buffer Buffer_A = cle.Push<float>(input_data, dims);
    cle::Buffer Buffer_B = cle.Create<float>(dims);

    // Call kernel
    cle.DilateSphere(Buffer_A, Buffer_B);  

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