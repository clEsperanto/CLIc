
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
    using TYPE = float;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->SetWaitForKernelToFinish(true);

    std::array<size_t,3> dims = {10, 1, 1};
    std::vector<TYPE> A (dims[0]*dims[1]*dims[2]); 
    std::fill (A.begin(),A.end(), 10.0f);

    auto img_A = gpu->Create<TYPE>(dims, "image");
    auto img_B = gpu->Push<TYPE>(A, dims, "image");
    auto C = gpu->Pull<TYPE>(img_B);

    if(IsDifferent(C,A))
    {
        return EXIT_FAILURE;
    }
    }


   {
    using TYPE = float;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->SetWaitForKernelToFinish(true);

    std::array<size_t,3> dims = {10, 5, 1};
    std::vector<TYPE> A (dims[0]*dims[1]*dims[2]); 
    std::fill (A.begin(),A.end(), 10.0f);

    auto img_A = gpu->Create<TYPE>(dims, "image");
    auto img_B = gpu->Push<TYPE>(A, dims, "image");
    auto C = gpu->Pull<TYPE>(img_B);
    
    if(IsDifferent(C,A))
    {
        return EXIT_FAILURE;
    }
    }

    {
    using TYPE = float;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->SetWaitForKernelToFinish(true);

    std::array<size_t,3> dims = {10, 5, 2};
    std::vector<TYPE> A (dims[0]*dims[1]*dims[2]); 
    std::fill (A.begin(),A.end(), 10.0f);

    auto img_A = gpu->Create<TYPE>(dims, "image");
    auto img_B = gpu->Push<TYPE>(A, dims, "image");
    auto C = gpu->Pull<TYPE>(img_B);
    
    if(IsDifferent(C,A))
    {
        return EXIT_FAILURE;
    }
    }

    {
    using TYPE = int;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->SetWaitForKernelToFinish(true);

    std::array<size_t,3> dims = {10, 5, 2};
    std::vector<TYPE> A (dims[0]*dims[1]*dims[2]); 
    std::fill (A.begin(),A.end(), 10.0f);

    auto img_A = gpu->Create<TYPE>(dims, "image");
    auto img_B = gpu->Push<TYPE>(A, dims, "image");
    auto C = gpu->Pull<TYPE>(img_B);
    
    if(IsDifferent(C,A))
    {
        return EXIT_FAILURE;
    }
    }

    {
    using TYPE = unsigned int;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->SetWaitForKernelToFinish(true);

    std::array<size_t,3> dims = {10, 5, 2};
    std::vector<TYPE> A (dims[0]*dims[1]*dims[2]); 
    std::fill (A.begin(),A.end(), 10.0f);

    auto img_A = gpu->Create<TYPE>(dims, "image");
    auto img_B = gpu->Push<TYPE>(A, dims, "image");
    auto C = gpu->Pull<TYPE>(img_B);
    
    if(IsDifferent(C,A))
    {
        return EXIT_FAILURE;
    }
    }


    return EXIT_SUCCESS;
}