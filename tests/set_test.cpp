
#include <random>
#include "clesperanto.hpp"

template<class type>
std::array<size_t,3> generate_data(std::vector<type>& arr_1, 
                                   std::vector<type>& valid, size_t width, size_t height, size_t depth, type scalar)
{
    arr_1.resize(width*height*depth);
    valid.resize(width*height*depth);
    std::fill(valid.begin(), valid.end(), scalar);
    for (auto it1 = arr_1.begin(); it1 != arr_1.end(); ++it1)
    {
        *it1 = static_cast<type>((int) rand() % 10);
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
std::vector<type> run_kernel_with_buffer(std::vector<type>& arr_1, std::array<size_t,3>& shape, type scalar)
{
    cle::Clesperanto cle;
    cle.Ressources()->SetWaitForKernelToFinish(true);
    auto ocl_output = cle.Push<type>(arr_1, shape);
    cle.Set(ocl_output, scalar);  
    auto output = cle.Pull<type>(ocl_output);  
    return output; 
}

template<class type>
std::vector<type> run_kernel_with_image(std::vector<type>& arr_1, std::array<size_t,3>& shape, type scalar)
{
    cle::Clesperanto cle;
    cle.Ressources()->SetWaitForKernelToFinish(true);
    auto ocl_output = cle.PushImage<type>(arr_1, shape);
    cle.Set(ocl_output, scalar);  
    auto output = cle.PullImage<type>(ocl_output);  
    return output; 
}

template<class type>
bool test(size_t width, size_t height, size_t depth)
{
    type scalar = static_cast<type>(rand() % 100);
    std::vector<type> arr_1, valid;
    std::array<size_t,3> shape = generate_data<type>(arr_1, valid, width, height, depth, scalar);
    auto output_buffer = run_kernel_with_buffer<type>(arr_1, shape, scalar);
    if (IsDifferent(output_buffer, valid))
    {
        std::cerr << "kernel ("<<width<<","<<height<<","<<depth<<") using buffer ... FAILED! " << std::endl;
        return true;
    }
    auto output_image  = run_kernel_with_image<type>(arr_1, shape, scalar);
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
        std::cerr << "Set kernel 3d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    if (test<float>(10, 5, 1))
    {
        std::cerr << "Set kernel 2d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    if (test<float>(10, 1, 1))
    {        
        std::cerr << "Set kernel 1d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Set kernel test ... PASSED! " << std::endl;
    return EXIT_SUCCESS;
}
