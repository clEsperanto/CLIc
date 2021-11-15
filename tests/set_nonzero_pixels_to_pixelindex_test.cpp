
#include <random>

#include "clesperanto.hpp"


int main(int argc, char **argv)
{
    // Test Initialisation
    using type = float;
    size_t width (4), height (4), depth (1);
    std::array<size_t,3> shape = {width, height, depth};
    
    std::vector<type> arr_in = {
                0, 0, 0, 1,
                0, 0, 3, 1,
                0, 0, 3, 1,
                1, 1, 1, 1
    };

    std::vector<type> arr_res = {
                0, 0, 0, 13,
                0, 0, 10, 14,
                0, 0, 11, 15,
                4, 8, 12, 16
    };


    // Test Kernel
    cle::Clesperanto cle;
    auto Buffer_A = cle.Push<type>(arr_in, shape);
    auto Buffer_B = cle.Create<type>(shape);
    cle.SetNonzeroPixelsToPixelindex(Buffer_A, Buffer_B);
    auto arr_out = cle.Pull<type>(Buffer_B);    

    // Test Validation
    float difference = 0;
    for( auto it1 = arr_res.begin(), it2 = arr_out.begin(); 
         it1 != arr_res.end() && it2 != arr_out.end(); ++it1, ++it2)
    {
        difference += std::abs(*it1 - *it2);
    }
    return difference > std::numeric_limits<type>::epsilon();
}