
#include <random>

#include "clesperanto.hpp"


int main(int argc, char **argv)
{
    // Test Initialisation
    using type = float;
    size_t width (10), height (10), depth (10);
    std::array<size_t,3> shape = {width, height, depth};
    std::vector<type> arr_in (width*height*depth);
    std::vector<type> arr_res (width*1*depth);
    std::fill(arr_in.begin(), arr_in.end(), 1.0f);
    std::fill(arr_res.begin(), arr_res.end(), 10.0f);

    // Test Kernel
    cle::Clesperanto cle;
    std::array<size_t,3> new_shape = {width, depth, 1};
    auto Buffer_A = cle.Push<type>(arr_in, shape);
    auto Buffer_B = cle.Create<type>(new_shape);
    cle.SumYProjection(Buffer_A, Buffer_B);   
    auto arr_out = cle.Pull<type>(Buffer_B);    

    // Test Validation
    float difference = 0;
    for (auto i = 0; i < arr_out.size(); ++i)
    {
        difference += std::abs(arr_res[i] - arr_out[i]);
    }
    return difference > std::numeric_limits<type>::epsilon();
}
