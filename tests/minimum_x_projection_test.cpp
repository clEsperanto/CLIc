
#include <random>

#include "clesperanto.hpp"


int main(int argc, char **argv)
{
    // Test Initialisation
    using type = float;
    size_t width (10), height (10), depth (10);
    std::array<size_t,3> shape = {width, height, depth};
    std::vector<type> arr_in (width*height*depth);
    std::vector<type> arr_res (width*height*1);
    std::default_random_engine generator;
    std::normal_distribution<type> distribution(5.0,2.0);
    for (int d = 0; d < depth; d++)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int i = x + width*(y+height*d);
                if ( x == y )
                {
                    arr_in[i] = distribution(generator);
                }
                else
                {
                    arr_in[i] = -1000;
                }
            }
        }
    }
    for (int i = 0; i < depth*height*1; ++i)
    {
        arr_res[i] = -1000;
    }

    // Test Kernel
    cle::Clesperanto cle;
    std::array<size_t,3> new_shape = {height, depth, 1};
    auto Buffer_A = cle.Push<type>(arr_in, shape);
    auto Buffer_B = cle.Create<type>(new_shape);
    cle.MinimumXProjection(Buffer_A, Buffer_B);   
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
