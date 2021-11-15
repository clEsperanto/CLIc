
#include <random>

#include "clesperanto.hpp"


int main(int argc, char **argv)
{
    // Test Initialisation
    using type = float;
    size_t width (12), height (1), depth (1);
    std::array<size_t,3> shape = {width, height, depth};
    std::vector<type> arr_in{ 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
    std::vector<type> arr_res{ 0.0f, 1.0f, 0.0f, 2.0f, 0.0f, 0.0f, 3.0f, 4.0f, 0.0f, 0.0f, 5.0f, 0.0f };
    int max_label = width - 1;
    int blocksize = 4;
    
    // Test Kernel
    cle::Clesperanto cle;
    auto gpuFlagIndices = cle.Push<type>(arr_in, shape);
    auto gpuNewIndices = cle.Create<type>(shape);
    size_t block_value =  static_cast<size_t>((static_cast<size_t>(max_label) + 1) / blocksize) + 1;
    std::array<size_t,3> block_dim = {block_value, 1, 1};
    auto gpuBlockSums = cle.Create<type>(block_dim);
    cle.SumReductionX(gpuFlagIndices, gpuBlockSums, blocksize);
    cle.BlockEnumerate(gpuFlagIndices, gpuBlockSums, gpuNewIndices, blocksize);
    auto arr_out = cle.Pull<type>(gpuNewIndices);    

    // Test Validation
    float difference = 0;
    for( auto it1 = arr_res.begin(), it2 = arr_out.begin(); 
         it1 != arr_res.end() && it2 != arr_out.end(); ++it1, ++it2)
    {
        difference += std::abs(*it1 - *it2);
    }
    return difference > std::numeric_limits<type>::epsilon();
}