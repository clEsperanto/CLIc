
#include <random>
#include <iostream>

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
    std::vector<type> arr_res = {
                0.0141675f, 0.0233582f, 0.0141675f,
                0.0233582f, 0.0385112f, 0.0233582f,
                0.0141675f, 0.0233582f, 0.0141675f,

                0.0233582f, 0.0385112f, 0.0233582f,
                0.0385112f, 0.0634942f, 0.0385112f,
                0.0233582f, 0.0385112f, 0.0233582f,

                0.0141675f, 0.0233582f, 0.0141675f,
                0.0233582f, 0.0385112f, 0.0233582f,
                0.0141675f, 0.0233582f, 0.0141675f
    };

    // Test Kernel
    cle::Clesperanto cle;
    auto Buffer_A = cle.PushImage<type>(arr_in, shape);
    auto Buffer_B = cle.CreateImage<type>(shape);
    cle.GaussianBlur(Buffer_A, Buffer_B, 1.0f, 1.0f, 1.0f);
    auto arr_out = cle.PullImage<type>(Buffer_B);  

    // Test Validation
    float difference = 0;
    for( auto it1 = arr_res.begin(), it2 = arr_out.begin(); 
         it1 != arr_res.end() && it2 != arr_out.end(); ++it1, ++it2)
    {
        difference += std::abs(*it1 - std::round(*it2 * 10000000.0f) / 10000000.0f);
    }
    return difference > std::numeric_limits<type>::epsilon();
}