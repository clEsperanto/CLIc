
#include <random>

#include "CLE.h"

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    // Initialise random input and valid output.
    unsigned int width (3), height (3), depth (2);
    float input_data[18] = {
                0, 0, 0,
                0, 1, 0,
                0, 0, 0,

                0, 0, 0,
                0, 0, 0,
                0, 0, 0
    };
    float valid_data[18] = {
                1, 1, 1,
                1, 1, 1,
                1, 1, 1,

                1, 1, 1,
                1, 1, 1,
                1, 1, 1
    };
    Image<float> input_img (input_data, width, height, depth, "float");

    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);

    // Initialise device memory and push from host to device
    cle::Buffer gpuInput = cle.Push<float>(input_data, dims);
    cle::Buffer gpuOutput = cle.Create<float>(gpuInput);

    cle.Maximum3DBox(gpuInput, gpuOutput, 1, 1, 1);

    // pull device memory to host
    std::vector<float> output_data = cle.Pull<float>(gpuOutput);    

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < width*height*depth; i++)
    {
        if (i % width == 0) {
            std::cout << std::endl;
        }
        std::cout << output_data[i] << " ";
        difference += std::abs(valid_data[i] - output_data[i]);
    }
    std::cout << std::endl;
    if (difference > std::numeric_limits<float>::epsilon())
    {
        std::cout << "Test failed, cumulated absolute difference " << difference << " > CPU epsilon (" << std::numeric_limits<float>::epsilon() << ")" << std::endl;
        return EXIT_FAILURE;
    }

    // That's all folks!
    std::cout << "Test succeded!"<< std::endl;
    return EXIT_SUCCESS;
}