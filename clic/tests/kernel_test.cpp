
#include "cleGPU.hpp"
#include "cleBuffer.hpp"
#include "cleImage.hpp"
#include "cleKernelList.hpp"

int main(int argc, char **argv)
{

    {
    using TYPE = float;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->WaitForKernelToFinish();

    std::array<size_t,3> dims = {10, 1, 1};
    std::vector<TYPE> data (dims[0]*dims[1]*dims[2]); 
    std::vector<TYPE> valid (dims[0]*dims[1]*dims[2]); 
    std::fill (data.begin(),data.end(), 10.0f);
    std::fill (valid.begin(),valid.end(), 100.0f);

    auto buff_A = gpu->PushBuffer<TYPE>(data, dims);
    auto buff_B = gpu->CreateBuffer<TYPE>(dims);

    cle::AddImageAndScalarKernel kernel(gpu);
    kernel.SetInput(buff_A);
    kernel.SetOutput(buff_B);
    kernel.SetScalar(90.0f);
    kernel.Execute();

    std::vector<TYPE> res = gpu->Pull<TYPE>(buff_B);
    
    float difference = 0;
    for( auto it1 = valid.begin(), it2 = res.begin(); 
         it1 != valid.end() && it2 != res.end(); ++it1, ++it2)
    {
        difference += std::abs(*it1 - *it2);
    }
    if (difference > std::numeric_limits<TYPE>::epsilon())
    {
        std::cout << "test kernel fail - diff = " << difference << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "test kernel pass - diff = " << difference << std::endl;
    }
    }

    {
    using TYPE = float;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->WaitForKernelToFinish();

    std::array<size_t,3> dims = {10, 5, 1};
    std::vector<TYPE> data (dims[0]*dims[1]*dims[2]); 
    std::vector<TYPE> valid (dims[0]*dims[1]*dims[2]); 
    std::fill (data.begin(),data.end(), 10.0f);
    std::fill (valid.begin(),valid.end(), 100.0f);

    auto buff_A = gpu->PushBuffer<TYPE>(data, dims);
    auto buff_B = gpu->CreateBuffer<TYPE>(dims);

    cle::AddImageAndScalarKernel kernel(gpu);
    kernel.SetInput(buff_A);
    kernel.SetOutput(buff_B);
    kernel.SetScalar(90.0f);
    kernel.Execute();

    std::vector<TYPE> res = gpu->Pull<TYPE>(buff_B);
    

    float difference = 0;
    for( auto it1 = valid.begin(), it2 = res.begin(); 
         it1 != valid.end() && it2 != res.end(); ++it1, ++it2)
    {
        difference += std::abs(*it1 - *it2);
    }
    if (difference > std::numeric_limits<TYPE>::epsilon())
    {
        std::cout << "test kernel fail - diff = " << difference << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "test kernel pass - diff = " << difference << std::endl;
    }
    }

    {
    using TYPE = float;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->WaitForKernelToFinish();

    std::array<size_t,3> dims = {10, 5, 2};
    std::vector<TYPE> data (dims[0]*dims[1]*dims[2]); 
    std::vector<TYPE> valid (dims[0]*dims[1]*dims[2]); 
    std::fill (data.begin(),data.end(), 10.0f);
    std::fill (valid.begin(),valid.end(), 100.0f);

    auto buff_A = gpu->PushBuffer<TYPE>(data, dims);
    auto buff_B = gpu->CreateBuffer<TYPE>(dims);

    cle::AddImageAndScalarKernel kernel(gpu);
    kernel.SetInput(buff_A);
    kernel.SetOutput(buff_B);
    kernel.SetScalar(90.0f);
    kernel.Execute();

    std::vector<TYPE> res = gpu->Pull<TYPE>(buff_B);
    

    float difference = 0;
    for( auto it1 = valid.begin(), it2 = res.begin(); 
         it1 != valid.end() && it2 != res.end(); ++it1, ++it2)
    {
        difference += std::abs(*it1 - *it2);
    }
    if (difference > std::numeric_limits<TYPE>::epsilon())
    {
        std::cout << "test kernel fail - diff = " << difference << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "test kernel pass - diff = " << difference << std::endl;
    }
    }

    {
    using TYPE = float;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->WaitForKernelToFinish();

    std::array<size_t,3> dims = {10, 5, 2};
    std::vector<TYPE> data (dims[0]*dims[1]*dims[2]); 
    std::vector<TYPE> valid (dims[0]*dims[1]*dims[2]); 
    std::fill (data.begin(),data.end(), 10.0f);
    std::fill (valid.begin(),valid.end(), 100.0f);

    auto image_A = gpu->PushImage<TYPE>(data, dims);
    auto image_B = gpu->CreateImage<TYPE>(dims);

    cle::AddImageAndScalarKernel kernel(gpu);
    kernel.SetInput(image_A);
    kernel.SetOutput(image_B);
    kernel.SetScalar(90.0f);
    kernel.Execute();

    std::vector<TYPE> res = gpu->Pull<TYPE>(image_B);
    

    float difference = 0;
    for( auto it1 = valid.begin(), it2 = res.begin(); 
         it1 != valid.end() && it2 != res.end(); ++it1, ++it2)
    {
        difference += std::abs(*it1 - *it2);
    }
    if (difference > std::numeric_limits<TYPE>::epsilon())
    {
        std::cout << "test kernel fail - diff = " << difference << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "test kernel pass - diff = " << difference << std::endl;
    }
    }
     
    {
    using TYPE = float;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->WaitForKernelToFinish();

    std::array<size_t,3> dims = {10, 5, 1};
    std::vector<TYPE> data (dims[0]*dims[1]*dims[2]); 
    std::vector<TYPE> valid (dims[0]*dims[1]*dims[2]); 
    std::fill (data.begin(),data.end(), 10.0f);
    std::fill (valid.begin(),valid.end(), 100.0f);

    auto image_A = gpu->PushImage<TYPE>(data, dims);
    auto image_B = gpu->CreateImage<TYPE>(dims);

    cle::AddImageAndScalarKernel kernel(gpu);
    kernel.SetInput(image_A);
    kernel.SetOutput(image_B);
    kernel.SetScalar(90.0f);
    kernel.Execute();

    std::vector<TYPE> res = gpu->Pull<TYPE>(image_B);
    

    float difference = 0;
    for( auto it1 = valid.begin(), it2 = res.begin(); 
         it1 != valid.end() && it2 != res.end(); ++it1, ++it2)
    {
        difference += std::abs(*it1 - *it2);
    }
    if (difference > std::numeric_limits<TYPE>::epsilon())
    {
        std::cout << "test kernel fail - diff = " << difference << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "test kernel pass - diff = " << difference << std::endl;
    }
    }

    {
    using TYPE = float;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->WaitForKernelToFinish();

    std::array<size_t,3> dims = {10, 1, 1};
    std::vector<TYPE> data (dims[0]*dims[1]*dims[2]); 
    std::vector<TYPE> valid (dims[0]*dims[1]*dims[2]); 
    std::fill (data.begin(),data.end(), 10.0f);
    std::fill (valid.begin(),valid.end(), 100.0f);

    auto image_A = gpu->PushImage<TYPE>(data, dims);
    auto image_B = gpu->CreateImage<TYPE>(dims);

    cle::AddImageAndScalarKernel kernel(gpu);
    kernel.SetInput(image_A);
    kernel.SetOutput(image_B);
    kernel.SetScalar(90.0f);
    kernel.Execute();

    std::vector<TYPE> res = gpu->Pull<TYPE>(image_B);
    

    float difference = 0;
    for( auto it1 = valid.begin(), it2 = res.begin(); 
         it1 != valid.end() && it2 != res.end(); ++it1, ++it2)
    {
        difference += std::abs(*it1 - *it2);
    }
    if (difference > std::numeric_limits<TYPE>::epsilon())
    {
        std::cout << "test kernel fail - diff = " << difference << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "test kernel pass - diff = " << difference << std::endl;
    }
    }

    return EXIT_SUCCESS;
}