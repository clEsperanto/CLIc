
#include <random>

#include "clesperanto.hpp"


int main(int argc, char **argv)
{
    // Test Initialisation
    using type = float;
    size_t width (4), height (3), depth (2);
    std::array<size_t,3> shape = {width, height, depth};
    std::vector<type> arr_in {
                1, 2, 3, 4,
                5, 6, 7, 8,
                9, 0, 1, 2,

                3, 4, 5, 6,
                7, 8, 9, 0,
                1, 2, 3, 4
    };

    std::vector<type> arr_res {
                2, 2, 2, 2,
                2, 2, 2, 2,
                2, 2, 2, 2,

                2, 2, 2, 2,
                2, 2, 2, 2,
                2, 2, 2, 2
    };


    // Test Kernel
    cle::Clesperanto cle;
    auto Buffer_A = cle.Push<type>(arr_in, shape);
    cle.Set(Buffer_A, 2);
    auto arr_out = cle.Pull<type>(Buffer_A);    


    // Test Validation
    float difference = 0;
    for( auto it1 = arr_res.begin(), it2 = arr_out.begin(); 
         it1 != arr_res.end() && it2 != arr_out.end(); ++it1, ++it2)
    {
        difference += std::abs(*it1 - *it2);
    }
    return difference > std::numeric_limits<type>::epsilon();
}