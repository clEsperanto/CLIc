
#include <random>

#include "clesperanto.hpp"


int main(int argc, char **argv)
{
    // Test Initialisation
    using type = float;
    size_t width (4), height (4), depth (2);
    std::array<size_t,3> shape = {width, height, depth};
    std::vector<type> arr_in {
                1, 2, 0, 0,
                3, 0, 0, 0,
                0, 0, 0, 6,
                0, 0, 0, 7,

                1, 2, 0, 3,
                3, 3, 0, 1,
                0, 3, 0, 6,
                7, 0, 0, 7
    };
    std::vector<type> arr_res {
                1, 1, 1, 1, 0, 0, 1, 1
    };


    // Test Kernel
    cle::Clesperanto cle;
    auto Buffer_A = cle.Push<type>(arr_in, shape);
    std::array<size_t,3> dimensions = {8, 1, 1}; //TODO: This should also work width flag depth=1, but it doesn't
    auto Buffer_B = cle.Create<type>(dimensions);
    cle.FlagExistingLabels(Buffer_A, Buffer_B);
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