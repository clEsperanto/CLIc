
#include <random>
#include <vector>
#include <iostream>

#include "CLE.h"

/**
 * Main test function
 *
 */
int main(int argc, char** argv)
{
    // Initialise random input and valid output.
    unsigned int width(5), height(5), depth(1);
    unsigned int dims[3] = {width, height, depth};
    std::vector<float> input_data (width*height*depth);
    std::vector<float> valid_data (width*height*depth);
    for (size_t i = 0; i < input_data.size(); i++)
    {
        if (i == 12) // center pixel
        {
            input_data[i] = 9;
        }
        else
        {
            input_data[i] = 0;
        }
        if ((i > 5 && i < 9) || (i > 10 && i < 14) || (i > 15 && i < 19)) 
        {
            valid_data[i] = 1;
        } 
        else 
        {
            valid_data[i] = 0;
        }
    }
    
    cle::GPU gpu;
    cle::CLE cle(gpu);

    cle::Buffer Buffer_A = cle.Push<float>(input_data, dims);
    cle::Buffer Buffer_B = cle.Create<float>(dims);

    cle.MeanBox(Buffer_A, Buffer_B, 1, 1);
    
    std::vector<float> output_data = cle.Pull<float>(Buffer_B);
    
    // Verify output
    float difference = 0;
    for (size_t i = 0; i < output_data.size(); i++)
    {
        if (i % width == 0) {
            std::cout << std::endl;
        }
        std::cout << output_data[i] << " ";
        difference += std::abs(valid_data[i] - output_data[i]);
    }
    std::cout << std::endl;
    return difference > std::numeric_limits<float>::epsilon();
}