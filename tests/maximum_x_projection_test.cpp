
#include <random>
#include <iostream>

#include "clesperanto.hpp"


int main(int argc, char **argv)
{
    // Test Initialisation
    using type = float;
    size_t width (3), height (3), depth (3);
    std::array<size_t,3> shape = {width, height, depth};
    std::vector<type> arr_in ({
        10.0f, 1.0f, 5.0f, 
         1.0f,10.0f, 6.0f, 
         5.0f, 8.0f,10.0f,

         1.0f, 1.0f,10.0f, 
         1.0f,10.0f, 6.0f, 
        10.0f, 8.0f, 4.0f,

        10.0f, 1.0f, 1.0f,
         1.0f, 1.0f,10.0f,
         5.0f,10.0f, 8.0f, 
    });
    std::vector<type> arr_res (1*height*depth);
    std::fill(arr_res.begin(), arr_res.end(), 10.0f);


    // Test Kernel
    cle::Clesperanto cle;
    auto Buffer_A = cle.Push<type>(arr_in, shape);
    std::array<size_t,3> new_shape = {height, depth, 1};
    auto Buffer_B = cle.Create<type>(new_shape);
    cle.MaximumXProjection(Buffer_A, Buffer_B);   
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
