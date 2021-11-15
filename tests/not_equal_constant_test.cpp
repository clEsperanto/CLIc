
#include <random>

#include "clesperanto.hpp"


int main(int argc, char **argv)
{
    // Test Initialisation
    using type = float;
    size_t width (10), height (10), depth (10);
    std::array<size_t,3> shape = {width, height, depth};
    std::vector<type> arr_in (width*height*depth);
    std::vector<type> arr_res (width*height*depth);
    float scalar = 50.0f;
    for (auto i = 0; i < arr_in.size(); ++i)
    {
        if (i%2 == 0)
        {
            arr_in[i] = 50;
            arr_res[i] = 0;
        }
        else
        {
            arr_in[i] = 0;
            arr_res[i] = 1;
        }
    }

    // Test Kernel
    cle::Clesperanto cle;
    auto Buffer_A = cle.Push<type>(arr_in, shape);
    auto Buffer_B = cle.Create<type>(shape);
    cle.NotEqualConstant(Buffer_A, Buffer_B, scalar);
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