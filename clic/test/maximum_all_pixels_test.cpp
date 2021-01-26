
#include <random>
#include <iostream>

#include "CLE.h"

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    // Initialise random input and valid output.
    unsigned int width (10), height (10), depth (10);
    std::vector<float> input_data (width*height*depth);
    std::vector<float> valid_data (1);
    std::default_random_engine generator;
    std::normal_distribution<float> distribution(5.0,2.0);
    for (size_t i = 0; i < width*height*depth; i++)
    {
        input_data[i] = distribution(generator);
    }
    input_data[50] = 1000;
    valid_data[0] = 1000;

    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);

    unsigned int dim[3] = {width, height, depth};
    cle::Buffer Buffer_A = cle.Push<float>(input_data, dim);
    unsigned int dim2[3] = {1, 1, 1};
    cle::Buffer Buffer_B = cle.Create<float>(valid_data, dim2);

    // Call kernel
    cle.MaximumOfAllPixels(Buffer_A, Buffer_B);   

    // pull device memory to host
    std::vector<float> ouput_data = cle.Pull<float>(Buffer_B);

    // Verify output
    float difference = std::abs(valid_data[0] - ouput_data[0]); 
    std::cout << "real maximum = " << valid_data[0] << ", computed maximum = " << ouput_data[0] << std::endl;
    if (difference > std::numeric_limits<float>::epsilon())
    {
        std::cout << "Test failled, cumulated absolute difference " << difference << " > CPU epsilon (" << std::numeric_limits<float>::epsilon() << ")" << std::endl;
        return EXIT_FAILURE;
    }
    
    // That's all folks!
    std::cout << "Test succeded!"<< std::endl;
    return EXIT_SUCCESS;
}
