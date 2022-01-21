
// #include <random>

// #include "clesperanto.hpp"


// int main(int argc, char **argv)
// {
//     // Test Initialisation
//     using type = float;
//     size_t width (12), height (1), depth (1);
//     std::array<size_t,3> shape = {width, height, depth};
//     std::vector<type> arr_in{ 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
//     std::vector<type> arr_res{ 0.0f, 1.0f, 0.0f, 2.0f, 0.0f, 0.0f, 3.0f, 4.0f, 0.0f, 0.0f, 5.0f, 0.0f };
//     int max_label = width - 1;
//     int blocksize = 4;
    
//     // Test Kernel
//      cle::Clesperanto cle;
//     auto gpuFlagIndices = cle.Push<type>(arr_in, shape);
//     auto gpuNewIndices = cle.Create<type>(shape);
//     size_t block_value =  static_cast<size_t>((static_cast<size_t>(max_label) + 1) / blocksize) + 1;
//     std::array<size_t,3> block_dim = {block_value, 1, 1};
//     auto gpuBlockSums = cle.Create<type>(block_dim);
//     cle.SumReductionX(gpuFlagIndices, gpuBlockSums, blocksize);
//     cle.BlockEnumerate(gpuFlagIndices, gpuBlockSums, gpuNewIndices, blocksize);
//     auto arr_out = cle.Pull<type>(gpuNewIndices);   

//     // Test Validation
//     float difference = 0;
//     for( auto it1 = arr_res.begin(), it2 = arr_out.begin(); 
//          it1 != arr_res.end() && it2 != arr_out.end(); ++it1, ++it2)
//     {
//         difference += std::abs(*it1 - *it2);
//     }
//     return difference > std::numeric_limits<type>::epsilon();
// }



#include <random>
#include "clesperanto.hpp"

template<class type>
std::array<size_t,3> generate_data(std::vector<type>& arr_1, 
                                   std::vector<type>& valid, size_t width, size_t height, size_t depth)
{
    arr_1.resize(width*height*depth);
    valid.resize(width*height*depth);
    type index = 0;
    for (auto it1 = arr_1.begin(),it_valid = valid.begin(); 
              it1 != arr_1.end(),it_valid != valid.end(); ++it1, ++it_valid)
    {
        if( (it1-arr_1.begin()) % 2 == 0)
        {
            *it_valid = ++index;
            *it1 = 1;
        }
        else
        {
            *it_valid = 0;
            *it1 = 0;
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
        difference += std::abs( static_cast<float>(*it_output) - static_cast<float>(*it_valid) );
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
    auto oclArray_A = cle.PushImage<type>(arr_1, shape);
    size_t block_value =  static_cast<size_t>((static_cast<size_t>(shape[0]-1) + 1) / blocksize) + 1;
    auto oclArray_B = cle.CreateImage<type>({block_value, 1, 1});
    auto ocl_output = cle.CreateImage<type>(shape);
    cle.SumReductionX(oclArray_A, oclArray_B, blocksize);
    cle.BlockEnumerate(oclArray_A, oclArray_B, ocl_output, blocksize);
    auto output = cle.PullImage<type>(ocl_output);  
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
    if (test<float>(12, 1, 1))
    {        
        std::cerr << "SumReductionX kernel 1d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "SumReductionX kernel test ... PASSED! " << std::endl;
    return EXIT_SUCCESS;
}

