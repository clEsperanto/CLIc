
#include <random>

#include "clesperanto.hpp"


int main(int argc, char **argv)
{
    // Test Initialisation
    using type = float;
    size_t width (10), height (10), depth (1);
    std::array<size_t,3> shape = {width, height, depth};  
    std::vector<type> arr_in {
                0.0f,  0.0f,  0.0f,  0.0f, 0.0f,  0.0f,  0.0f,  0.0f,  0.0f, 0.0f, 
                0.0f, 12.0f,  0.0f,  0.0f, 0.0f, 52.0f, 52.0f, 52.0f,  0.0f, 0.0f, 
                0.0f, 12.0f,  0.0f, 33.0f, 0.0f, 52.0f,  0.0f, 52.0f,  0.0f, 0.0f, 
                0.0f,  0.0f,  0.0f, 33.0f, 0.0f, 52.0f, 52.0f, 52.0f,  0.0f, 0.0f, 
                0.0f,  0.0f,  0.0f,  0.0f, 0.0f,  0.0f,  0.0f,  0.0f,  0.0f, 0.0f, 
                0.0f,  0.0f,  0.0f,  0.0f, 0.0f,  0.0f,  0.0f, 67.0f,  0.0f, 0.0f, 
                0.0f, 17.0f,  0.0f,  0.0f, 0.0f,  0.0f, 67.0f, 67.0f, 67.0f, 0.0f, 
                0.0f, 17.0f, 17.0f, 17.0f, 0.0f,  0.0f,  0.0f, 67.0f,  0.0f, 0.0f, 
                0.0f,  0.0f,  0.0f, 17.0f, 0.0f,  0.0f, 67.0f,  0.0f, 67.0f, 0.0f, 
                0.0f,  0.0f,  0.0f,  0.0f, 0.0f,  0.0f,  0.0f,  0.0f,  0.0f, 0.0f
    };
    std::vector<type> arr_res {
                0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 4.0f, 4.0f, 4.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 3.0f, 0.0f, 4.0f, 0.0f, 4.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 3.0f, 0.0f, 4.0f, 4.0f, 4.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.0f, 0.0f, 0.0f,
                0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.0f, 5.0f, 5.0f, 0.0f,
                0.0f, 2.0f, 2.0f, 2.0f, 0.0f, 0.0f, 0.0f, 5.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 5.0f, 0.0f, 5.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
    };


    // Test Kernel
    cle::Clesperanto cle;
    auto Buffer_A = cle.Push<type>(arr_in, shape);
    auto Buffer_B = cle.Create<type>(shape);
    cle.CloseIndexGapsInLabelMap(Buffer_A, Buffer_B, 4096);
    auto arr_out = cle.Pull<type>(Buffer_B);    

    // Test Validation
    float difference = 0;
    for( auto it1 = arr_res.begin(), it2 = arr_out.begin(); 
         it1 != arr_res.end() && it2 != arr_out.end(); ++it1, ++it2)
    {
        difference += std::abs(*it1 - *it2);
    }
    DEBUG("\n difference = " << difference << "\n");
    return difference > std::numeric_limits<type>::epsilon();
}