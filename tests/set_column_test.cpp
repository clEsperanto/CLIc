
#include <random>

#include "clesperanto.hpp"


int main(int argc, char **argv)
{
    // Test Initialisation
    using type = float;
    size_t width (5), height (1), depth (1);
    std::array<size_t,3> shape = {width, height, depth};
    std::vector<type> arr_in (width*height*depth);
    std::vector<type> arr_res (width*height*depth);
    for (auto i = 0; i < arr_in.size(); ++i)
    {
            arr_in[i] = static_cast<type>(i);
            arr_res[i] = static_cast<type>(i);
    }
    arr_res[2] = 10.0f;

    // Test Kernel
    cle::Clesperanto cle;
    auto Buffer_A = cle.Push<type>(arr_in, shape);
    cle.SetColumn(Buffer_A, 2, 10);
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