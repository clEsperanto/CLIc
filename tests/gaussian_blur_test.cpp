
// #include <random>
// #include <iostream>

// #include "clesperanto.hpp"


// int main(int argc, char **argv)
// {
//     // Test Initialisation
//     using type = float;
//     size_t width (3), height (3), depth (3);
//     std::array<size_t,3> shape = {width, height, depth};
//     std::vector<type> arr_in (width*height*depth);
//     std::fill(arr_in.begin(), arr_in.end(), 0.0f);
//     arr_in[13] = 100.0f;
//     std::vector<type> arr_res = {
//                 0.0141675f, 0.0233582f, 0.0141675f,
//                 0.0233582f, 0.0385112f, 0.0233582f,
//                 0.0141675f, 0.0233582f, 0.0141675f,

//                 0.0233582f, 0.0385112f, 0.0233582f,
//                 0.0385112f, 0.0634942f, 0.0385112f,
//                 0.0233582f, 0.0385112f, 0.0233582f,

//                 0.0141675f, 0.0233582f, 0.0141675f,
//                 0.0233582f, 0.0385112f, 0.0233582f,
//                 0.0141675f, 0.0233582f, 0.0141675f
//     };

//     // Test Kernel
//     cle::Clesperanto cle;
//     auto Buffer_A = cle.PushImage<type>(arr_in, shape);
//     auto Buffer_B = cle.CreateImage<type>(shape);
//     cle.GaussianBlur(Buffer_A, Buffer_B, 1.0f, 1.0f, 1.0f);
//     auto arr_out = cle.PullImage<type>(Buffer_B);  

//     // Test Validation
//     float difference = 0;
//     for( auto it1 = arr_res.begin(), it2 = arr_out.begin(); 
//          it1 != arr_res.end() && it2 != arr_out.end(); ++it1, ++it2)
//     {
//         difference += std::abs(*it1 - std::round(*it2 * 10000000.0f) / 10000000.0f);
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
    std::fill(arr_1.begin(), arr_1.end(), 0.0f);
    int central_idx = (width/2) + (height/2)*width + (depth/2) * height * width;
    arr_1[central_idx] = 100.0f;
    if(valid.size() == 27)
        valid = {
            1.41673, 2.33580, 1.41673,
            2.33580, 3.85108, 2.33580,
            1.41673, 2.33580, 1.41673,

            2.33580, 3.85108, 2.33580,
            3.85108, 6.34936, 3.85108,
            2.33580, 3.85108, 2.33580,

            1.41673, 2.33580, 1.41673,
            2.33580, 3.85108, 2.33580,
            1.41673, 2.33580, 1.41673
            };

    if(valid.size() == 9)
        valid = {
            5.8550,  9.6532, 5.8550,
            9.6532, 15.9155, 9.6532,
            5.8550,  9.6532, 5.8550
        };

    if(valid.size() == 3)
        valid = {
            24.1971, 39.8942, 24.1971 
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
        difference += std::abs( round(static_cast<float>(*it_output)*1000)/1000 - round(static_cast<float>(*it_valid)*1000)/1000);
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
    cle.GaussianBlur(oclArray_A, ocl_output, 1, 1, 1);  
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
    cle.GaussianBlur(oclArray_A, ocl_output, 1, 1, 1);  
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
        std::cerr << "GaussianBlur kernel 3d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    if (test<float>(3, 3, 1))
    {
        std::cerr << "GaussianBlur kernel 2d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    if (test<float>(3, 1, 1))
    {        
        std::cerr << "GaussianBlur kernel 1d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "GaussianBlur kernel test ... PASSED! " << std::endl;
    return EXIT_SUCCESS;
}
