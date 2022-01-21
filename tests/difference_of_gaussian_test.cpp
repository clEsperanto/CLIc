
#include <random>
#include "clesperanto.hpp"

template<class type>
std::array<size_t,3> generate_data(std::vector<type>& arr_1, 
                                   std::vector<type>& valid, size_t width, size_t height, size_t depth)
{
    arr_1.resize(width*height*depth);
    valid.resize(width*height*depth);
    std::fill(arr_1.begin(), arr_1.end(), 0.0f);
    int central_idx = (width/2) + (height/2)*width + (depth/2) * height * width;
    arr_1[central_idx] = 100.0f;
    if(valid.size() == 27)
        valid = {
            1.21767, 2.12536, 1.21767,
            2.12536, 3.62863, 2.12536,
            1.21767, 2.12536, 1.21767,

            2.12536, 3.62863, 2.12536,
            3.62863, 6.11420, 3.62863,
            2.12536, 3.62863, 2.12536,

            1.21767, 2.12536, 1.21767,
            2.12536, 3.62863, 2.12536,
            1.21767, 2.12536, 1.21767
            };

    if(valid.size() == 9)
        valid = {
            4.2725,  7.9804, 4.2725,
            7.9804, 14.1471, 7.9804,
            4.2725,  7.9804, 4.2725
        };

    if(valid.size() == 3)
        valid = {
            11.6176, 26.5961, 11.6176 
        };

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
        difference += std::abs( round(static_cast<float>(*it_output)*100)/100 - round(static_cast<float>(*it_valid)*100)/100);
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
    auto oclArray_A = cle.Push<type>(arr_1, shape);
    auto ocl_output = cle.Create<type>(shape);
    cle.DifferenceOfGaussian(oclArray_A, ocl_output, 1, 1, 1, 3, 3, 3);  
    auto output = cle.Pull<type>(ocl_output);  
    return output; 
}

template<class type>
std::vector<type> run_kernel_with_image(std::vector<type>& arr_1, std::array<size_t,3>& shape)
{
    cle::Clesperanto cle;
    cle.Ressources()->SetWaitForKernelToFinish(true);
    auto oclArray_A = cle.PushImage<type>(arr_1, shape);
    auto ocl_output = cle.CreateImage<type>(shape);
    cle.DifferenceOfGaussian(oclArray_A, ocl_output, 1, 1, 1, 3, 3, 3);  
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
    if (test<float>(3, 3, 3))
    {
        std::cerr << "DifferenceOfGaussian kernel 3d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    if (test<float>(3, 3, 1))
    {
        std::cerr << "DifferenceOfGaussian kernel 2d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    if (test<float>(3, 1, 1))
    {        
        std::cerr << "DifferenceOfGaussian kernel 1d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "DifferenceOfGaussian kernel test ... PASSED! " << std::endl;
    return EXIT_SUCCESS;
}

