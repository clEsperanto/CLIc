
#include <random>
#include "clesperanto.hpp"
#include "utils.hpp"

template<class type>
std::array<size_t,3> generate_data(std::vector<type>& arr_1, 
                                   std::vector<type>& arr_2, std::vector<type>& valid, size_t width, size_t height, size_t depth, float scalar_1, float scalar_2)
{
    arr_1.resize(width*height*depth);
    arr_2.resize(width*height*depth);
    valid.resize(width*height*depth);
    type value_1 = static_cast<type>(rand() % 100);
    type value_2 = static_cast<type>(rand() % 100);
    type value_3 = static_cast<type>(value_1*scalar_1 + value_2*scalar_2);
    std::fill(arr_1.begin(), arr_1.end(), value_1);
    std::fill(arr_2.begin(), arr_2.end(), value_2);
    std::fill(valid.begin(), valid.end(), value_3);
    return std::array<size_t,3> {width, height, depth};
}

template<class type>
std::vector<type> run_kernel_with_buffer(std::vector<type>& arr_1, 
                                         std::vector<type>& arr_2, std::array<size_t,3>& shape, float scalar_1, float scalar_2)
{
    cle::Clesperanto cle;
    cle.Ressources()->SetWaitForKernelToFinish(true);
    auto oclArray_A = cle.Push<type>(arr_1, shape);
    auto oclArray_B = cle.Push<type>(arr_2, shape);
    auto ocl_output = cle.Create<type>(shape);
    cle.AddImagesWeighted(oclArray_A, oclArray_B, ocl_output, scalar_1, scalar_2);  
    auto output = cle.Pull<type>(ocl_output);  
    return output; 
}

template<class type>
std::vector<type> run_kernel_with_image(std::vector<type>& arr_1, 
                                        std::vector<type>& arr_2, std::array<size_t,3>& shape, float scalar_1, float scalar_2)
{
    cle::Clesperanto cle;
    cle.Ressources()->SetWaitForKernelToFinish(true);
    auto oclArray_A = cle.Push<type>(arr_1, shape, "image");
    auto oclArray_B = cle.Push<type>(arr_2, shape, "image");
    auto ocl_output = cle.Create<type>(shape, "image");
    cle.AddImagesWeighted(oclArray_A, oclArray_B, ocl_output, scalar_1, scalar_2);  
    auto output = cle.Pull<type>(ocl_output);  
    return output; 
}

template<class type>
bool test(size_t width, size_t height, size_t depth)
{
    float scalar_1 (rand()/RAND_MAX), scalar_2 (rand()/RAND_MAX);
    std::vector<type> arr_1, arr_2, valid;
    std::array<size_t,3> shape = generate_data<type>(arr_1, arr_2, valid, width, height, depth, scalar_1, scalar_2);
    auto output_buffer = run_kernel_with_buffer<type>(arr_1, arr_2, shape, scalar_1, scalar_2);
    if (IsDifferent(output_buffer, valid))
    {
        std::cerr << "kernel ("<<width<<","<<height<<","<<depth<<") using buffer ... FAILED! " << std::endl;
        return true;
    }
    auto output_image  = run_kernel_with_image<type>(arr_1, arr_2, shape, scalar_1, scalar_2);
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
        std::cerr << "AddImagesWeighted kernel 3d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    if (test<float>(10, 5,  1))
    {
        std::cerr << "AddImagesWeighted kernel 2d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    if (test<float>(10,  1,  1))
    {        
        std::cerr << "AddImagesWeighted kernel 1d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "AddImagesWeighted kernel test ... PASSED! " << std::endl;
    return EXIT_SUCCESS;
}