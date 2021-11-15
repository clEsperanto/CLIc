
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
    std::fill(arr_in.begin(), arr_in.end(), 0.0f);
    std::fill(arr_res.begin(), arr_res.end(), 0.0f);
    int central_idx = (width/2) + (height/2)*height + (depth/2) * height * width;
    arr_res[central_idx] = 1;
    arr_in[central_idx] = 1;
    arr_in[central_idx+1] = 1;
    arr_in[central_idx-1] = 1;
    arr_in[central_idx+height] = 1;
    arr_in[central_idx-height] = 1;
    arr_in[central_idx+(height * width)] = 1;
    arr_in[central_idx-(height * width)] = 1;

    // Test Kernel
    cle::Clesperanto cle;
    auto Buffer_A = cle.Push<type>(arr_in, shape);
    auto Buffer_B = cle.Create<type>(shape);
    cle.ErodeSphere(Buffer_A, Buffer_B);  
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