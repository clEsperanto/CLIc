
#include <random>

#include "clesperanto.hpp"


int main(int argc, char **argv)
{
    // Test Initialisation
    using type = float;
    size_t width (5), height (5), depth (1);
    std::array<size_t,3> shape = {width, height, depth};
    std::vector<type> arr_in (width*height*depth);
    std::vector<type> arr_res (width*height*depth);
    for (auto i = 0; i < arr_in.size(); ++i)
    {
        arr_in[i] = 0;
        if(i == 2*width+2)
        {
            arr_in[i] = 1;
        }

        if(i < 6 || i > 19)
        {
            arr_res[i] = 0;
        }
        else if(i%5 == 0 || i%5 == 4)
        {
            arr_res[i] = 0;
        }
        else
        {
            arr_res[i] = sqrt(2.0f);
        } 
    }
    arr_res[2*width+2] = 0;
    arr_res[3*width+2] = 2;
    arr_res[1*width+2] = 2;
    arr_res[2*width+3] = 2;
    arr_res[2*width+1] = 2;

    // Test Kernel
    cle::Clesperanto cle;
    auto Buffer_A = cle.Push<type>(arr_in, shape);
    auto Buffer_B = cle.Create<type>(shape);
    cle.Sobel(Buffer_A, Buffer_B);  
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