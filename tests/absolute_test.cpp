
#include <random>

#include "clesperanto.hpp"

template<class type>
std::vector<type> run_kernel_with_buffer(std::vector<type>& arr, std::array<size_t,3>& shape)
{
    cle::Clesperanto cle;
    auto oclArray_A = cle.Push<type>(arr, shape);
    auto oclArray_B = cle.Create<type>(shape);
    cle.Absolute(oclArray_A, oclArray_B);  
    auto result = cle.Pull<type>(oclArray_B);  
    return result; 
}

template<class type>
std::vector<type> run_kernel_with_image(std::vector<type>& arr, std::array<size_t,3>& shape)
{
    cle::Clesperanto cle;
    auto oclArray_A = cle.PushImage<type>(arr, shape);
    auto oclArray_B = cle.CreateImage<type>(shape);
    cle.Absolute(oclArray_A, oclArray_B);  
    auto result = cle.PullImage<type>(oclArray_B);  
    return result; 
}

template<class type>
bool IsDifferent(std::vector<type>& arr_1, std::vector<type>& arr_2)
{
    float difference = 0;
    for (auto it1 = arr_1.begin(), it2 = arr_2.begin(); 
              it1 != arr_1.end() && it2 != arr_2.end(); ++it1, ++it2)
    {
        difference += std::abs(static_cast<float>(*it1) - static_cast<float>(*it2));
    }
    if (difference != 0)
    {
        std::cerr << "[FAILED] : difference = " << difference << std::endl;
        return true;
    }
    else
    {
        return false;
    }
}

int main(int argc, char **argv)
{
    using type = float;
    size_t width (10), height (10), depth (10);
    std::array<size_t,3> shape = {width, height, depth};    
    std::vector<type> arr_in (width*height*depth);
    std::vector<type> arr_res (width*height*depth);
    for (auto i = 0; i < arr_in.size(); ++i)
    {
        if (i % 2 == 0)
        {
            arr_in[i] = -1.0f;
        }
        else
        {
            arr_in[i] = 1.0f;
        }
        arr_res[i] = 1.0f;
    }

    auto arr_buffer = run_kernel_with_buffer<type>(arr_in, shape);
    if (IsDifferent(arr_buffer, arr_res))
    {
        std::cerr << "Absolute kernel using buffer ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }

    auto arr_image  = run_kernel_with_image<type>(arr_in, shape);
    if (IsDifferent(arr_image, arr_res))
    {
        std::cerr << "Absolute kernel using image ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Absolute kernel test ... PASSED!" << std::endl;
    return EXIT_SUCCESS;
}