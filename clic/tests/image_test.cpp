
#include <random>
#include <iostream>
#include <vector>

#include "cleGPU.hpp"
#include "cleImage.hpp"


/**
 * Main test function
 *
 */

int main(int argc, char **argv)
{
    {
    using TYPE = float;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->WaitForKernelToFinish();

    std::array<size_t,3> dims = {10, 1, 1};
    std::vector<TYPE> A (dims[0]*dims[1]*dims[2]); 
    std::fill (A.begin(),A.end(), 10.0f);

    cle::Image img_A = gpu->CreateImage<TYPE>(dims);
    std::cout << "\tcreated image: " << img_A.Info() << std::endl;
    cle::Image img_B = gpu->PushImage<TYPE>(A, dims);
    std::cout << "\tpushed image: " << img_B.Info() << std::endl;
    std::vector<TYPE> C = gpu->Pull<TYPE>(img_B);
    std::cout << "\tpull image: " << img_B.Info() << std::endl;

    float difference = 0;
    for( auto it1 = A.begin(), it2 = C.begin(); 
         it1 != A.end() && it2 != C.end(); ++it1, ++it2)
    {
        difference += std::abs(*it1 - *it2);
    }
    if (difference > std::numeric_limits<TYPE>::epsilon())
    {
        std::cout << "test cleImage {Create, Push, Pull} fail - diff = " << difference << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "test cleImage {Create, Push, Pull} pass - diff = " << difference << std::endl;
    }
    }


   {
    using TYPE = float;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->WaitForKernelToFinish();

    std::array<size_t,3> dims = {10, 5, 1};
    std::vector<TYPE> A (dims[0]*dims[1]*dims[2]); 
    std::fill (A.begin(),A.end(), 10.0f);

    cle::Image img_A = gpu->CreateImage<TYPE>(dims);
    std::cout << "\tcreated image: " << img_A.Info() << std::endl;
    cle::Image img_B = gpu->PushImage<TYPE>(A, dims);
    std::cout << "\tpushed image: " << img_B.Info() << std::endl;
    std::vector<TYPE> C = gpu->Pull<TYPE>(img_B);
    std::cout << "\tpull image: " << img_B.Info() << std::endl;
    
    float difference = 0;
    for( auto it1 = A.begin(), it2 = C.begin(); 
         it1 != A.end() && it2 != C.end(); ++it1, ++it2)
    {
        difference += std::abs(*it1 - *it2);
    }
    if (difference > std::numeric_limits<TYPE>::epsilon())
    {
        std::cout << "test cleImage {Create, Push, Pull} fail - diff = " << difference << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "test cleImage {Create, Push, Pull} pass - diff = " << difference << std::endl;
    }
    }

    {
    using TYPE = float;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->WaitForKernelToFinish();

    std::array<size_t,3> dims = {10, 5, 2};
    std::vector<TYPE> A (dims[0]*dims[1]*dims[2]); 
    std::fill (A.begin(),A.end(), 10.0f);

    cle::Image img_A = gpu->CreateImage<TYPE>(dims);
    std::cout << "\tcreated image: " << img_A.Info() << std::endl;
    cle::Image img_B = gpu->PushImage<TYPE>(A, dims);
    std::cout << "\tpushed image: " << img_B.Info() << std::endl;
    std::vector<TYPE> C = gpu->Pull<TYPE>(img_B);
    std::cout << "\tpull image: " << img_B.Info() << std::endl;
    
    float difference = 0;
    for( auto it1 = A.begin(), it2 = C.begin(); 
         it1 != A.end() && it2 != C.end(); ++it1, ++it2)
    {
        difference += std::abs(*it1 - *it2);
    }
    if (difference > std::numeric_limits<TYPE>::epsilon())
    {
        std::cout << "test cleImage {Create, Push, Pull} fail - diff = " << difference << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "test cleImage {Create, Push, Pull} pass - diff = " << difference << std::endl;
    }
    }

    {
    using TYPE = int;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->WaitForKernelToFinish();

    std::array<size_t,3> dims = {10, 5, 2};
    std::vector<TYPE> A (dims[0]*dims[1]*dims[2]); 
    std::fill (A.begin(),A.end(), 10.0f);

    cle::Image img_A = gpu->CreateImage<TYPE>(dims);
    std::cout << "\tcreated image: " << img_A.Info() << std::endl;
    cle::Image img_B = gpu->PushImage<TYPE>(A, dims);
    std::cout << "\tpushed image: " << img_B.Info() << std::endl;
    std::vector<TYPE> C = gpu->Pull<TYPE>(img_B);
    std::cout << "\tpull image: " << img_B.Info() << std::endl;
    
    float difference = 0;
    for( auto it1 = A.begin(), it2 = C.begin(); 
         it1 != A.end() && it2 != C.end(); ++it1, ++it2)
    {
        difference += std::abs(*it1 - *it2);
    }
    if (difference > std::numeric_limits<TYPE>::epsilon())
    {
        std::cout << "test cleImage {Create, Push, Pull} fail - diff = " << difference << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "test cleImage {Create, Push, Pull} pass - diff = " << difference << std::endl;
    }
    }

    {
    using TYPE = unsigned int;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->WaitForKernelToFinish();

    std::array<size_t,3> dims = {10, 5, 2};
    std::vector<TYPE> A (dims[0]*dims[1]*dims[2]); 
    std::fill (A.begin(),A.end(), 10.0f);

    cle::Image img_A = gpu->CreateImage<TYPE>(dims);
    std::cout << "\tcreated image: " << img_A.Info() << std::endl;
    cle::Image img_B = gpu->PushImage<TYPE>(A, dims);
    std::cout << "\tpushed image: " << img_B.Info() << std::endl;
    std::vector<TYPE> C = gpu->Pull<TYPE>(img_B);
    std::cout << "\tpull image: " << img_B.Info() << std::endl;
    
    float difference = 0;
    for( auto it1 = A.begin(), it2 = C.begin(); 
         it1 != A.end() && it2 != C.end(); ++it1, ++it2)
    {
        difference += *it1 - *it2;
    }
    if (difference > std::numeric_limits<TYPE>::epsilon())
    {
        std::cout << "test cleImage {Create, Push, Pull} fail - diff = " << difference << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "test cleImage {Create, Push, Pull} pass - diff = " << difference << std::endl;
    }
    }


    return EXIT_SUCCESS;
}