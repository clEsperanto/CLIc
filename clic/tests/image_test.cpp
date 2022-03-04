
#include <random>
#include <iostream>
#include <vector>

#include "cleGPU.hpp"
#include "utils.hpp"


/**
 * Main test function
 *
 */

int main(int argc, char **argv)
{
    {
    using type = float;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->SetWaitForKernelToFinish(true);

    std::array<size_t,3> dims = {10, 1, 1};
    std::vector<type> A (dims[0]*dims[1]*dims[2]); 
    std::fill (A.begin(),A.end(), static_cast<type>(10));

    auto img_A = gpu->Create<type>(dims, "image");
    auto img_B = gpu->Push<type>(A, dims, "image");
    auto C = gpu->Pull<type>(img_B);

    if(IsDifferent(C,A))
    {
        return EXIT_FAILURE;
    }
    }


   {
    using type = float;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->SetWaitForKernelToFinish(true);

    std::array<size_t,3> dims = {10, 5, 1};
    std::vector<type> A (dims[0]*dims[1]*dims[2]); 
    std::fill (A.begin(),A.end(), static_cast<type>(10));

    auto img_A = gpu->Create<type>(dims, "image");
    auto img_B = gpu->Push<type>(A, dims, "image");
    auto C = gpu->Pull<type>(img_B);
    
    if(IsDifferent(C,A))
    {
        return EXIT_FAILURE;
    }
    }

    {
    using type = float;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->SetWaitForKernelToFinish(true);

    std::array<size_t,3> dims = {10, 5, 2};
    std::vector<type> A (dims[0]*dims[1]*dims[2]); 
    std::fill (A.begin(),A.end(), static_cast<type>(10));

    auto img_A = gpu->Create<type>(dims, "image");
    auto img_B = gpu->Push<type>(A, dims, "image");
    auto C = gpu->Pull<type>(img_B);
    
    if(IsDifferent(C,A))
    {
        return EXIT_FAILURE;
    }
    }

    {
    using type = int;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->SetWaitForKernelToFinish(true);

    std::array<size_t,3> dims = {10, 5, 2};
    std::vector<type> A (dims[0]*dims[1]*dims[2]); 
    std::fill (A.begin(),A.end(), static_cast<type>(10));

    auto img_A = gpu->Create<type>(dims, "image");
    auto img_B = gpu->Push<type>(A, dims, "image");
    auto C = gpu->Pull<type>(img_B);
    
    if(IsDifferent(C,A))
    {
        return EXIT_FAILURE;
    }
    }

    {
    using type = unsigned int;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->SetWaitForKernelToFinish(true);

    std::array<size_t,3> dims = {10, 5, 2};
    std::vector<type> A (dims[0]*dims[1]*dims[2]); 
    std::fill (A.begin(),A.end(), static_cast<type>(10));

    auto img_A = gpu->Create<type>(dims, "image");
    auto img_B = gpu->Push<type>(A, dims, "image");
    auto C = gpu->Pull<type>(img_B);
    
    if(IsDifferent(C,A))
    {
        return EXIT_FAILURE;
    }
    }


    return EXIT_SUCCESS;
}