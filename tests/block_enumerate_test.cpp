
#include <random>
#include "clesperanto.hpp"
#include "utils.hpp"

template<class type>
std::array<size_t,3> generate_data(std::vector<type>& arr_1, 
                                   std::vector<type>& valid, size_t width, size_t height, size_t depth)
{
    arr_1.resize(width*height*depth);
    valid.resize(width*height*depth);
    type index = 0;
    for (auto it1 = arr_1.begin(),it_valid = valid.begin(); 
              (it1 != arr_1.end()) && (it_valid != valid.end()); ++it1, ++it_valid)
    {
        if( (it1-arr_1.begin()) % 2 == 0)
        {
            *it_valid = static_cast<type>(++index);
            *it1 = static_cast<type>(1);
        }
        else
        {
            *it_valid = static_cast<type>(0);
            *it1 = static_cast<type>(0);
        }
    }

    return std::array<size_t,3> {width, height, depth};
}



template<class type>
std::vector<type> run_kernel_with_buffer(std::vector<type>& arr_1, std::array<size_t,3>& shape)
{
    cle::Clesperanto cle;
    cle.Ressources()->SetWaitForKernelToFinish(true);
    int blocksize = 4;
    auto oclArray_A = cle.Push<type>(arr_1, shape);
    size_t block_value =  static_cast<size_t>((static_cast<size_t>(shape[0]-1) + 1) / blocksize) + 1;
    auto oclArray_B = cle.Create<type>({block_value, 1, 1});
    auto ocl_output = cle.Create<type>(shape);
    cle.SumReductionX(oclArray_A, oclArray_B, blocksize);
    cle.BlockEnumerate(oclArray_A, oclArray_B, ocl_output, blocksize);
    auto output = cle.Pull<type>(ocl_output);  
    return output; 
}

template<class type>
std::vector<type> run_kernel_with_image(std::vector<type>& arr_1, std::array<size_t,3>& shape)
{
    cle::Clesperanto cle;
    cle.Ressources()->SetWaitForKernelToFinish(true);
    int blocksize = 4;
    auto oclArray_A = cle.Push<type>(arr_1, shape, "image");
    size_t block_value =  static_cast<size_t>((static_cast<size_t>(shape[0]-1) + 1) / blocksize) + 1;
    auto oclArray_B = cle.Create<type>({block_value, 1, 1}, "image");
    auto ocl_output = cle.Create<type>(shape, "image");
    cle.SumReductionX(oclArray_A, oclArray_B, blocksize);
    cle.BlockEnumerate(oclArray_A, oclArray_B, ocl_output, blocksize);
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
    // auto output_image  = run_kernel_with_image<type>(arr_1, shape);
    // if (IsDifferent(output_image, valid))
    // {
    //     std::cerr << "kernel ("<<width<<","<<height<<","<<depth<<") using image ... FAILED! " << std::endl;
    //     return true;
    // }
    return false;
}

int main(int argc, char **argv)
{
    if (test<float>(12, 1, 1))
    {        
        std::cerr << "BlockEnumerate kernel 1d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "BlockEnumerate kernel test ... PASSED! " << std::endl;
    return EXIT_SUCCESS;
}

