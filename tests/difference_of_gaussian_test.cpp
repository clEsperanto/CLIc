
#include <random>

#include "clesperanto.hpp"


int main(int argc, char **argv)
{
    // Test Initialisation
    using type = float;
    size_t width (3), height (3), depth (3);
    std::array<size_t,3> shape = {width, height, depth};
    std::vector<type> arr_in (width*height*depth);
    std::fill(arr_in.begin(), arr_in.end(), 0.0f);
    arr_in[13] = 1.0f;
    std::vector<type>  arr_res {
                0.003464f, 0.004077f, 0.003464f,
                0.004077f, 0.004780f, 0.004077f,
                0.003464f, 0.004077f, 0.003464f,

                0.004077f, 0.004780f, 0.004077f,
                0.004780f, 0.005585f, 0.004780f,
                0.004077f, 0.004780f, 0.004077f,

                0.003464f, 0.004077f, 0.003464f,
                0.004077f, 0.004780f, 0.004077f,
                0.003464f, 0.004077f, 0.003464f  
    };

    // Test Kernel
    cle::Clesperanto cle;
    auto Buffer_A = cle.Push<type>(arr_in, shape);
    auto Buffer_B = cle.Create<type>(shape);
    cle.DifferenceOfGaussian(Buffer_A, Buffer_B, 2.0f, 2.0f, 2.0f, 3.0f, 3.0f, 3.0f);
    auto arr_out = cle.Pull<type>(Buffer_B);    

    // Test Validation
    float difference = 0;
    for( auto it1 = arr_res.begin(), it2 = arr_out.begin(); 
         it1 != arr_res.end() && it2 != arr_out.end(); ++it1, ++it2)
    {
        difference += std::abs(*it1 - std::round(*it2 * 1000000.0f) / 1000000.0f);
    }
    return difference > std::numeric_limits<type>::epsilon();
}