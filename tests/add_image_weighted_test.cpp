
#include <random>

#include "clesperanto.hpp"


int main(int argc, char **argv)
{
    // Test Initialisation
    using type = float;
    size_t width (3), height (3), depth (3);
    std::array<size_t,3> shape = {width, height, depth};
    std::vector<type> arr_in1 (width*height*depth);
    std::fill(arr_in1.begin(), arr_in1.end(), 10.0f);
    std::vector<type> arr_in2 (width*height*depth);
    std::fill(arr_in2.begin(), arr_in2.end(), 10.0f);
    std::vector<type> arr_res (width*height*depth);
    std::fill(arr_res.begin(), arr_res.end(), 15.0f);

    // Test Kernel
    cle::Clesperanto cle;
    auto Buffer_A = cle.Push<type>(arr_in1, shape);
    auto Buffer_B = cle.Push<type>(arr_in2, shape);
    auto Buffer_C = cle.Create<type>(shape);
    cle.AddImagesWeighted(Buffer_A, Buffer_B, Buffer_C, 1, 0.5f);
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