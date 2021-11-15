
#include <random>

#include "clesperanto.hpp"


int main(int argc, char **argv)
{
    // Test Initialisation
    using type = float;
    size_t width (4), height (4), depth (1);
    std::array<size_t,3> shape = {width, height, depth};
    std::vector<type> arr_in {
                1.0f, 2.0f, 0.0f, 0.0f,
                3.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 6.0f,
                0.0f, 0.0f, 0.0f, 7.0f
    };

    std::vector<type> arr_res {
                1.0f, 1.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 6.0f,
                0.0f, 0.0f, 0.0f, 6.0f
    };


    // Test Kernel
    cle::Clesperanto cle;
    auto Buffer_A = cle.Push<type>(arr_in, shape);
    std::array<size_t,3> dimensions = {1, 1, 2}; //TODO: This should also work width flag depth=1, but it doesn't
    auto Buffer_B = cle.Create<type>(dimensions);
    auto Buffer_C = cle.Create<type>(shape);
    cle.NonzeroMinimumBox(Buffer_A, Buffer_B, Buffer_C);
    auto arr_out = cle.Pull<type>(Buffer_C);    


    // Test Validation
    float difference = 0;
    for( auto it1 = arr_res.begin(), it2 = arr_out.begin(); 
         it1 != arr_res.end() && it2 != arr_out.end(); ++it1, ++it2)
    {
        difference += std::abs(*it1 - *it2);
    }
    return difference > std::numeric_limits<type>::epsilon();
}