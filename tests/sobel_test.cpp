
#include <random>

#include "CLE.hpp"

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    // Initialise random input and valid output.
    unsigned int width (5), height (5), depth (1);
    unsigned int dims[3] = {width, height, depth};
    std::vector<float> input_data (width*height*depth);
    std::vector<float> valid_data (width*height*depth);
    for (size_t i = 0; i < width*height*depth; i++)
    {
        if (i % width == 0) {
            std::cout << std::endl;
        }

        input_data[i] = 0;
        if(i == 2*width+2)
        {
            input_data[i] = 1;
        }

        if(i < 6 || i > 19)
        {
            valid_data[i] = 0;
        }
        else if(i%5 == 0 || i%5 == 4)
        {
            valid_data[i] = 0;
        }
        else
        {
            valid_data[i] = sqrt(2);
        } 
        std::cout << input_data[i] << " ";
    }
    std::cout << std::endl;

    valid_data[2*width+2] = 0;
    valid_data[3*width+2] = 2;
    valid_data[1*width+2] = 2;
    valid_data[2*width+3] = 2;
    valid_data[2*width+1] = 2;

    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);
    
    // Initialise device memory and push from host
    cle::Buffer Buffer_A = cle.Push<float>(input_data, dims);
    cle::Buffer Buffer_B = cle.Create<float>(dims);

    // Call kernel
    cle.Sobel(Buffer_A, Buffer_B);  

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