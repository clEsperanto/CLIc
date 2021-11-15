
#include <random>

#include "clesperanto.hpp"


int main(int argc, char **argv)
{
    // Test Initialisation
    using type = float;
    size_t width (4), height (3), depth (2);
    std::array<size_t,3> shape = {width, height, depth};
    std::vector<type> arr_in1 {
                1, 2, 3, 4,
                5, 6, 7, 8,
                9, 0, 1, 2,

                3, 4, 5, 6,
                7, 8, 9, 0,
                1, 2, 3, 4
    };
    std::vector<type> arr_in2 (width*height*depth);
    std::vector<type> arr_res {
                0, 0, 0, 0,
                1, 0, 0, 0,
                0, 0, 0, 0,

                0, 0, 1, 0,
                0, 0, 0, 0,
                0, 0, 0, 0
    };


    // Test Kernel
    cle::Clesperanto cle;
    auto Buffer_A = cle.Push<type>(arr_in1, shape);
    auto Buffer_B = cle.Push<type>(arr_in1, shape);
    cle.Set(Buffer_B, 5);
    auto Buffer_C = cle.Create<type>(shape);
    cle.Equal(Buffer_A, Buffer_B, Buffer_C);
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