
#include <random>
#include "clesperanto.hpp"

template<class type>
std::array<size_t,3> generate_data(std::vector<type>& arr_1, std::vector<type>& valid, size_t width, size_t height, size_t depth, float scalar_1, float scalar_2)
{
    arr_1.resize(width*height*depth);
    valid.resize(width*height*depth);
    std::fill(valid.begin(), valid.end(), 0);
    type value = 0;
    for (auto it1 = arr_1.begin(), it_valid = valid.begin(); 
              it1 != arr_1.end(), it_valid != valid.end(); ++it1, ++it_valid)
    {
        if ( (it1 - arr_1.begin()) % 2 == 0)
        {
            *it1 = scalar_1;
            *it_valid = scalar_2;
        }
        else
        {
            value = static_cast<type>((int) rand() % 4);
            *it1 = value;
            *it_valid = value;
        }
    }
    return std::array<size_t,3> {width, height, depth};
}

template<class type>
bool IsDifferent(std::vector<type>& output, std::vector<type>& valid)
{
    if (output.size() != valid.size())
    {
        std::cerr << "[FAILED] : output size does not match." << std::endl;
        return true;
    }
    float difference = 0;
    for (auto it_output = output.begin(), it_valid = valid.begin(); 
              it_output != output.end(), it_valid != valid.end(); ++it_output, ++it_valid)
    {
        difference += std::abs(static_cast<float>(*it_output) - static_cast<float>(*it_valid));
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

template<class type>
std::vector<type> run_kernel_with_buffer(std::vector<type>& arr, std::array<size_t,3>& shape, float scalar_1, float scalar_2)
{
    cle::Clesperanto cle;
    cle.Ressources()->SetWaitForKernelToFinish(true);
    auto oclArray_A = cle.Push<type>(arr, shape);
    auto ocl_output = cle.Create<type>(shape);
    cle.ReplaceIntensity(oclArray_A, ocl_output, scalar_1, scalar_2);  
    auto output = cle.Pull<type>(ocl_output);  
    return output; 
}

template<class type>
std::vector<type> run_kernel_with_image(std::vector<type>& arr, std::array<size_t,3>& shape, float scalar_1, float scalar_2)
{
    cle::Clesperanto cle;
    cle.Ressources()->SetWaitForKernelToFinish(true);
    auto oclArray_A = cle.PushImage<type>(arr, shape);
    auto ocl_output = cle.CreateImage<type>(shape);
    cle.ReplaceIntensity(oclArray_A, ocl_output, scalar_1, scalar_2);  
    auto output = cle.PullImage<type>(ocl_output);  
    return output; 
}

template<class type>
bool test(size_t width, size_t height, size_t depth)
{
    float scalar_1 (rand() % 100), scalar_2 (rand() % 100);
    std::vector<type> arr_1, valid;
    std::array<size_t,3> shape = generate_data<type>(arr_1, valid, width, height, depth, scalar_1, scalar_2);
    auto output_buffer = run_kernel_with_buffer<type>(arr_1, shape, scalar_1, scalar_2);
    if (IsDifferent(output_buffer, valid))
    {
        std::cerr << "kernel ("<<width<<","<<height<<","<<depth<<") using buffer ... FAILED! " << std::endl;
        return true;
    }
    auto output_image = run_kernel_with_image<type>(arr_1, shape, scalar_1, scalar_2);
    if (IsDifferent(output_image, valid))
    {
        std::cerr << "kernel ("<<width<<","<<height<<","<<depth<<") using image ... FAILED! " << std::endl;
        return true;
    }
    return false;
}

int main(int argc, char **argv)
{
    if (test<float>(10, 5, 2))
    {
        std::cerr << "ReplaceIntensity kernel 3d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    if (test<float>(10, 5,  1))
    {
        std::cerr << "ReplaceIntensity kernel 2d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    if (test<float>(10,  1,  1))
    {        
        std::cerr << "ReplaceIntensity kernel 1d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "ReplaceIntensity kernel test ... PASSED! " << std::endl;
    return EXIT_SUCCESS;
}