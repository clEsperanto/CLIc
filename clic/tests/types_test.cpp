

#include "CLE.h"


template<class T>
bool ApplyFilter(cle::CLE cle, unsigned int dims[3], float scalar)
{
    std::vector<T> input_data (dims[0]*dims[1]*dims[2]);
    std::vector<T> valid_data (dims[0]*dims[1]*dims[2]);
    std::fill(input_data.begin(), input_data.end(), 5);
    std::fill(valid_data.begin(), valid_data.end(), 5 + scalar);

    cle::Buffer Buffer_A = cle.Push<T>(input_data, dims);
    cle::Buffer Buffer_B = cle.Create<T>(dims);

    cle.AddImageAndScalar(Buffer_A, Buffer_B, scalar);

    std::vector<T> output_data = cle.Pull<T>(Buffer_B);

    float difference = 0;
    for (size_t i = 0; i < output_data.size(); i++)
    {
        difference += std::abs((float)valid_data[i] - output_data[i]);
    }
    return difference > std::numeric_limits<float>::epsilon();
}

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    unsigned int width (10), height (10), depth (10);
    unsigned int dims[3] = {width, height, depth};

    cle::GPU gpu;
    cle::CLE cle(gpu);

    if (ApplyFilter<float>(cle, dims, 5))
    {
        std::cout<< "fail1 float" << std::endl;
        return EXIT_FAILURE;
    }
    // if (ApplyFilter<int>(cle, dims, 5))
    // {
    //     std::cout<< "fail1 int" << std::endl;
    //     return EXIT_FAILURE;
    // }
    // if (ApplyFilter<unsigned int>(cle, dims, 5))
    // {
    //     std::cout<< "fail1 usinged int" << std::endl;
    //     return EXIT_FAILURE;
    // }
    if (ApplyFilter<char>(cle, dims, 5))
    {
        std::cout<< "fail1 char" << std::endl;
        return EXIT_FAILURE;
    }
    if (ApplyFilter<unsigned char>(cle, dims, 5))
    {
        std::cout<< "fail1 unsigned char" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}