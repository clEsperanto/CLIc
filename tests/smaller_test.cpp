
#include <random>

#include "clesperanto.hpp"


int main(int argc, char **argv)
{
    // Test Initialisation
    using type = float;
    size_t width (4), height (3), depth (2);
    std::array<size_t,3> shape = {width, height, depth};
    std::vector<type> arr_in1 {
                1.0f, 2.0f, 3.0f, 4.0f,
                5.0f, 6.0f, 7.0f, 8.0f,
                9.0f, 0.0f, 1.0f, 2.0f,

                3.0f, 4.0f, 5.0f, 6.0f,
                7.0f, 8.0f, 9.0f, 0.0f,
                1.0f, 2.0f, 3.0f, 4.0f
    };
    std::vector<type> arr_in2 (width*height*depth);
    std::vector<type> arr_res {
                1.0f, 1.0f, 1.0f, 1.0f,
                0.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 1.0f, 1.0f,

                1.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 1.0f
    };


    // Test Kernel
    cle::Clesperanto cle;
    auto Buffer_A = cle.Push<type>(arr_in1, shape);
    auto Buffer_B = cle.Push<type>(arr_in2, shape);
    cle.Set(Buffer_B, 5);
    auto Buffer_C = cle.Create<type>(shape);
    cle.Smaller(Buffer_A, Buffer_B, Buffer_C);
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