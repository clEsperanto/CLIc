
#include <random>
#include <vector>
#include <iostream>

#include "CLE.h"

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    unsigned int width (10), height (10), depth (10);
    unsigned int dims[3] = {width, height, depth};
    std::vector<float> input_data (width*height*depth);
    std::vector<float> valid_data (width*height*depth);
    std::vector<float> ouput_data (width*height*depth);
    float scalar = 100;
    std::default_random_engine generator;
    std::normal_distribution<float> distribution(5.0,2.0);
    for (size_t i = 0; i < input_data.size(); i++)
    {
        input_data[i] = distribution(generator);
        valid_data[i] = input_data[i] + scalar;
    }

    cle::GPU gpu;
    cle::CLE cle(gpu);

    cle::Buffer Buffer_A = cle.Push<float>(input_data, dims);
    cle::Buffer Buffer_B = cle.Create<float>(dims);

    cle.AddImageAndScalar(Buffer_A, Buffer_B, scalar);

    std::vector<float> output_data = cle.Pull<float>(Buffer_B);

    float difference = 0;
    for (size_t i = 0; i < output_data.size(); i++)
    {
        difference += std::abs(valid_data[i] - ouput_data[i]);
    }
    if (difference > std::numeric_limits<float>::epsilon())
    {
        std::cout << "Test failled, cumulated absolute difference " << difference << " > CPU epsilon (" << std::numeric_limits<float>::epsilon() << ")" << std::endl;
        return EXIT_FAILURE;
    }

    // That's all folks!
    std::cout << "Test succeded!"<< std::endl;
    return EXIT_SUCCESS;
}