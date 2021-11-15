
#include <random>

#include "clesperanto.hpp"


int main(int argc, char **argv)
{
    // Test Initialisation
    using type = float;
    size_t width (12), height (1), depth (1);
    std::array<size_t,3> shape = {width, height, depth};
    int block_size = 4;
    std::vector<type> arr_in {0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0};
    std::vector<type> arr_res {2, 2, 1};


    // Test Kernel
    cle::Clesperanto cle;
    auto Buffer_A = cle.Push<type>(arr_in, shape);
    size_t sum_dim = static_cast<size_t>(width / block_size);
    std::array<size_t,3> new_shape = {sum_dim, 1, 1};
    auto Buffer_B = cle.Create<type>(new_shape);
    cle.SumReductionX(Buffer_A, Buffer_B, block_size);  
    auto arr_out = cle.Pull<type>(Buffer_B);    

    // Test Validation
    float difference = 0;
    for (auto i = 0; i < arr_out.size(); ++i)
    {
        difference += std::abs(arr_res[i] - arr_out[i]);
    }    
    return difference > std::numeric_limits<type>::epsilon();
}