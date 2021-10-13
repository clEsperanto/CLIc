
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
    std::vector<float> valid_data (width*height*depth);
    std::fill(input_data.begin(), input_data.end(), 0.0f);
    std::fill(valid_data.begin(), valid_data.end(), 0.0f);

    int central_idx = (width/2) + (height/2)*height + (depth/2) * height * width;
    valid_data[central_idx] = 1;
    input_data[central_idx] = 1;
    input_data[central_idx+1] = 1;
    input_data[central_idx-1] = 1;
    input_data[central_idx+height] = 1;
    input_data[central_idx-height] = 1;
    input_data[central_idx+(height * width)] = 1;
    input_data[central_idx-(height * width)] = 1;

    // Initialise GPU information.
    cle::Clesperanto cle;
    
    // Initialise device memory and push from host
    cle::Buffer Buffer_A = cle.Push<float>(input_data, dims);
    cle::Buffer Buffer_B = cle.Create<float>(dims);

    // Call kernel
    cle.ErodeSphere(Buffer_A, Buffer_B);  

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