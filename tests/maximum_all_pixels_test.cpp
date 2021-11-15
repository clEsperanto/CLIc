
#include <random>
#include <iostream>

#include "clesperanto.hpp"


int main(int argc, char **argv)
{
    // Test Initialisation
    using type = float;
    size_t width (10), height (10), depth (10);
    std::array<size_t,3> shape = {width, height, depth};
    std::vector<type> arr_in (width*height*depth);
    std::vector<type> arr_res (1);
    std::default_random_engine generator;
    std::normal_distribution<type> distribution(5.0f, 2.0f);
    for (auto i = 0; i < arr_in.size(); ++i)
    {
        arr_in[i] = distribution(generator);
    }
    arr_in[50] = 1000.0f;
    arr_res[0] = 1000.0f;

    // Test Kernel
    cle::Clesperanto cle;
    auto Buffer_A = cle.PushImage<type>(arr_in, shape);
    auto Buffer_B = cle.CreateImage<type>();
    cle.MaximumOfAllPixels(Buffer_A, Buffer_B);   
    auto arr_out = cle.PullImage<type>(Buffer_B);

    float difference = std::abs(arr_res.front() - arr_out.front()); 
    return difference > std::numeric_limits<type>::epsilon();
}
