
#include "clesperanto.hpp"

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    // Initialise random input and valid output.
    int width (4), height (3), depth (2);
    int dims[3] = {width, height, depth};
    std::vector<float> input_data = {
                1, 2, 3, 4,
                5, 6, 7, 8,
                9, 0, 1, 2,

                3, 4, 5, 6,
                7, 8, 9, 0,
                1, 2, 3, 4
    };
    std::vector<float> valid_data = {
                0, 0, 0, 0,
                1, 1, 1, 1,
                1, 0, 0, 0,

                0, 0, 1, 1,
                1, 1, 1, 0,
                0, 0, 0, 0
    };

    // Initialise GPU information.
    cle::Clesperanto cle;

    // Initialise device memory and push from host to device
    cle::Buffer Buffer_A = cle.Push<float>(input_data, dims);
    cle::Buffer Buffer_B = cle.Create<float>(dims);
    cle.Set(Buffer_B, 5);

    cle::Buffer Buffer_C = cle.Create<float>(Buffer_A);

    // Call kernel
    cle.GreaterOrEqual(Buffer_A, Buffer_B, Buffer_C);

    // pull device memory to host
    std::vector<float> output_data = cle.Pull<float>(Buffer_C);    

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < width*height*depth; i++)
    {
        difference += std::abs(valid_data[i] - output_data[i]);
    }
    return difference > std::numeric_limits<float>::epsilon();
}