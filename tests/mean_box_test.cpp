
#include <random>
#include <vector>
#include <iostream>

#include "clesperanto.hpp"


int main(int argc, char** argv)
{
    // Test Initialisation
    using type = float;
    size_t width(5), height(5), depth(1);
    std::array<size_t,3> shape = {width, height, depth};
    std::vector<type> arr_in (width*height*depth);
    std::vector<type> arr_res (width*height*depth);
    for (auto i = 0; i < arr_in.size(); ++i)
    {
        if (i == 12) // center pixel
        {
            arr_in[i] = 9.0f;
        }
        else
        {
            arr_in[i] = 0.0f;
        }
        if ((i > 5 && i < 9) || (i > 10 && i < 14) || (i > 15 && i < 19)) 
        {
            arr_res[i] = 1.0f;
        } 
        else 
        {
            arr_res[i] = 0.0f;
        }
    }
    
    // Test Kernel
    cle::Clesperanto cle;
    auto Buffer_A = cle.Push<type>(arr_in, shape);
    auto Buffer_B = cle.Create<type>(shape);
    cle.MeanBox(Buffer_A, Buffer_B, 1, 1);
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