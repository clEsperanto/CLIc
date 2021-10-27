
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
    std::array<int,3> dims = {width, height, depth};
    std::vector<float> input_data (width*height*depth);
    std::vector<float> valid_data (width*height*1);
    std::default_random_engine generator;
    std::normal_distribution<float> distribution(5.0,2.0);
    for (int d = 0; d < depth; d++)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int i = x + width*(y+height*d);
                if ( x == y )
                {
                    input_data[i] = distribution(generator);
                }
                else
                {
                    input_data[i] = -1000;
                }
            }
        }
    }
    for (int i = 0; i < width*depth*1; i++)
    {
        valid_data[i] = -1000;
    }

    // Initialise GPU information.
    cle::Clesperanto cle;

    // Initialise device memory and push from host
    std::array<int, 3> new_dims = {width, depth, 1};
    cle::Buffer Buffer_A = cle.Push<float>(input_data, dims);
    cle::Buffer Buffer_B = cle.Create<float>(new_dims);

    // Call kernel
    cle.MinimumYProjection(Buffer_A, Buffer_B);   

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
