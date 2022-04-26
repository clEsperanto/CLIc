#include <random>
#include "clesperanto.hpp"
#include "utils.hpp"

template<class type>
std::array<size_t,3> generate_data(std::vector<type>& arr_1, 
                                   std::vector<type>& valid, size_t width, size_t height, size_t depth)
{
    arr_1.resize(width*height*depth);
    valid.resize(5);
    arr_1 = {1,2,3,1,2,3,5,2,1,3,2,1};
    valid = {4,4,3,0,1};
    return std::array<size_t,3> {width, height, depth};
}

template<class type>
std::vector<type> run_kernel_with_buffer(std::vector<type>& arr_1, std::array<size_t,3>& shape)
{
    cle::Clesperanto cle;
    cle.Ressources()->SetWaitForKernelToFinish(true);
    auto oclArray_A = cle.Push<type>(arr_1, shape);
    auto ocl_output = cle.Create<type>({5,1,1});
    cle.Histogram(oclArray_A, ocl_output, 5);  
    auto output = cle.Pull<type>(ocl_output); 
    return output; 
}

template<class type>
std::vector<type> run_kernel_with_image(std::vector<type>& arr_1, std::array<size_t,3>& shape)
{
    cle::Clesperanto cle;
    cle.Ressources()->SetWaitForKernelToFinish(true);
    auto oclArray_A = cle.Push<type>(arr_1, shape, "image");
    auto ocl_output = cle.Create<type>({5,1,1}, "image");
    cle.Histogram(oclArray_A, ocl_output, 5);  
    auto output = cle.Pull<type>(ocl_output); 
    return output; 
}

template<class type>
bool test(size_t width, size_t height, size_t depth)
{
    std::vector<type> arr_1, valid;
    std::array<size_t,3> shape = generate_data<type>(arr_1, valid, width, height, depth);
    auto output_buffer = run_kernel_with_buffer<type>(arr_1, shape);
    if (IsDifferent(output_buffer, valid))
    {
        std::cerr << "kernel ("<<width<<","<<height<<","<<depth<<") using buffer ... FAILED! " << std::endl;
        return true;
    }
    auto output_image  = run_kernel_with_image<type>(arr_1, shape);
    if (IsDifferent(output_image, valid))
    {
        std::cerr << "kernel ("<<width<<","<<height<<","<<depth<<") using image ... FAILED! " << std::endl;
        return true;
    }
    return false;
}

int main(int argc, char **argv)
{
    if (test<float>(3, 2, 2))
    {
        std::cerr << "Histogram kernel 3d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    if (test<float>(6, 2, 1))
    {
        std::cerr << "Histogram kernel 2d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    if (test<float>(12, 1, 1))
    {        
        std::cerr << "Histogram kernel 1d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Histogram kernel test ... PASSED! " << std::endl;
    return EXIT_SUCCESS;
}