
#include <random>
#include "clesperanto.hpp"
#include "utils.hpp"

template<class type>
std::array<size_t,3> generate_data(std::vector<type>& arr_1, 
                                   std::vector<type>& valid, size_t width, size_t height, size_t depth, int scalar_1, type scalar_2)
{
    arr_1.resize(width*height*depth);
    valid.resize(width*height*depth);
    for (auto it1 = arr_1.begin(), it_valid = valid.begin(); 
              (it1 != arr_1.end()) && (it_valid != valid.end()); ++it1, ++it_valid)
    {
        *it1 = static_cast<type>((int) rand() % 10);
        *it_valid = *it1;
        if( (it1 - arr_1.begin()) % width == scalar_1 )
        {
            *it_valid = scalar_2;
        }
    }
    return std::array<size_t,3> {width, height, depth};
}

template<class type>
std::vector<type> run_kernel_with_buffer(std::vector<type>& arr_1, std::array<size_t,3>& shape, int scalar_1, type scalar_2)
{
    cle::Clesperanto cle;
    cle.Ressources()->SetWaitForKernelToFinish(true);
    auto ocl_output = cle.Push<type>(arr_1, shape);
    cle.SetColumn(ocl_output, scalar_1, scalar_2);  
    auto output = cle.Pull<type>(ocl_output);  
    return output; 
}

template<class type>
std::vector<type> run_kernel_with_image(std::vector<type>& arr_1, std::array<size_t,3>& shape, int scalar_1, type scalar_2)
{
    cle::Clesperanto cle;
    cle.Ressources()->SetWaitForKernelToFinish(true);
    auto ocl_output = cle.Push<type>(arr_1, shape, "image");
    cle.SetColumn(ocl_output, scalar_1, scalar_2);  
    auto output = cle.Pull<type>(ocl_output);  
    return output; 
}

template<class type>
bool test(size_t width, size_t height, size_t depth)
{
    int scalar_1 = static_cast<int>(rand() % width);
    type scalar_2 = static_cast<type>(rand() % 100);
    std::vector<type> arr_1, valid;
    std::array<size_t,3> shape = generate_data<type>(arr_1, valid, width, height, depth, scalar_1, scalar_2);
    auto output_buffer = run_kernel_with_buffer<type>(arr_1, shape, scalar_1, scalar_2);
    if (IsDifferent(output_buffer, valid))
    {
        std::cerr << "kernel ("<<width<<","<<height<<","<<depth<<") using buffer ... FAILED! " << std::endl;
        return true;
    }
    auto output_image  = run_kernel_with_image<type>(arr_1, shape, scalar_1, scalar_2);
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
        std::cerr << "SetColumn kernel 3d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    if (test<float>(10, 5, 1))
    {
        std::cerr << "SetColumn kernel 2d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    if (test<float>(10, 1, 1))
    {        
        std::cerr << "SetColumn kernel 1d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "SetColumn kernel test ... PASSED! " << std::endl;
    return EXIT_SUCCESS;
}
