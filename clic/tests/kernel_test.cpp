
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

    std::array<int,3> dims = {10, 1, 1};
    std::vector<TYPE> data (dims[0]*dims[1]*dims[2]); 
    std::vector<TYPE> valid (dims[0]*dims[1]*dims[2]); 
    std::fill (data.begin(),data.end(), 10.0f);
    std::fill (valid.begin(),valid.end(), 100.0f);

    cle::Buffer buff_A = gpu->PushBuffer<TYPE>(data, dims);
    cle::Buffer buff_B = gpu->CreateBuffer<TYPE>(dims);

    cle::AddImageAndScalarKernel kernel(gpu);
    kernel.SetInput(buff_A);
    kernel.SetOutput(buff_B);
    kernel.SetScalar(90.0f);
    kernel.Execute();

    std::vector<TYPE> res = gpu->Pull<TYPE>(buff_B);
    

    float diff = 0;
    for (int i =0; i< res.size(); i++)
    {
        diff += valid[i] - res[i];
    }
    if (diff > 0)
    {
        std::cout << "test kernel fail - diff = " << diff << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "test kernel pass - diff = " << diff << std::endl;
    }
    }

    {
    using TYPE = float;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->WaitForKernelToFinish();

    std::array<int,3> dims = {10, 5, 2};
    std::vector<TYPE> data (dims[0]*dims[1]*dims[2]); 
    std::vector<TYPE> valid (dims[0]*dims[1]*dims[2]); 
    std::fill (data.begin(),data.end(), 10.0f);
    std::fill (valid.begin(),valid.end(), 100.0f);

    cle::Image image_A = gpu->PushImage<TYPE>(data, dims);
    cle::Image image_B = gpu->CreateImage<TYPE>(dims);

    cle::AddImageAndScalarKernel kernel(gpu);
    kernel.SetInput(image_A);
    kernel.SetOutput(image_B);
    kernel.SetScalar(90.0f);
    kernel.Execute();

    std::vector<TYPE> res = gpu->Pull<TYPE>(image_B);
    

    float diff = 0;
    for (int i =0; i< res.size(); i++)
    {
        diff += valid[i] - res[i];
    }
    if (diff > 0)
    {
        std::cout << "test kernel fail - diff = " << diff << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "test kernel pass - diff = " << diff << std::endl;
    }
    }
     
    {
    using TYPE = float;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->WaitForKernelToFinish();

    std::array<int,3> dims = {10, 5, 1};
    std::vector<TYPE> data (dims[0]*dims[1]*dims[2]); 
    std::vector<TYPE> valid (dims[0]*dims[1]*dims[2]); 
    std::fill (data.begin(),data.end(), 10.0f);
    std::fill (valid.begin(),valid.end(), 100.0f);

    cle::Image image_A = gpu->PushImage<TYPE>(data, dims);
    cle::Image image_B = gpu->CreateImage<TYPE>(dims);

    cle::AddImageAndScalarKernel kernel(gpu);
    kernel.SetInput(image_A);
    kernel.SetOutput(image_B);
    kernel.SetScalar(90.0f);
    kernel.Execute();

    std::vector<TYPE> res = gpu->Pull<TYPE>(image_B);
    

    float diff = 0;
    for (int i =0; i< res.size(); i++)
    {
        diff += valid[i] - res[i];
    }
    if (diff > 0)
    {
        std::cout << "test kernel fail - diff = " << diff << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "test kernel pass - diff = " << diff << std::endl;
    }
    }

    {
    using TYPE = float;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->WaitForKernelToFinish();

    std::array<int,3> dims = {10, 1, 1};
    std::vector<TYPE> data (dims[0]*dims[1]*dims[2]); 
    std::vector<TYPE> valid (dims[0]*dims[1]*dims[2]); 
    std::fill (data.begin(),data.end(), 10.0f);
    std::fill (valid.begin(),valid.end(), 100.0f);

    cle::Image image_A = gpu->PushImage<TYPE>(data, dims);
    cle::Image image_B = gpu->CreateImage<TYPE>(dims);

    cle::AddImageAndScalarKernel kernel(gpu);
    kernel.SetInput(image_A);
    kernel.SetOutput(image_B);
    kernel.SetScalar(90.0f);
    kernel.Execute();

    std::vector<TYPE> res = gpu->Pull<TYPE>(image_B);
    

    float diff = 0;
    for (int i =0; i< res.size(); i++)
    {
        diff += valid[i] - res[i];
    }
    if (diff > 0)
    {
        std::cout << "test kernel fail - diff = " << diff << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "test kernel pass - diff = " << diff << std::endl;
    }
    }

    {
    using TYPE = float;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->WaitForKernelToFinish();

    std::array<int,3> dims = {10, 1, 1};
    std::vector<TYPE> data (dims[0]*dims[1]*dims[2]); 
    std::vector<TYPE> valid (dims[0]*dims[1]*dims[2]); 
    std::fill (data.begin(),data.end(), 10.0f);
    std::fill (valid.begin(),valid.end(), 100.0f);

    cle::Image image_A = gpu->PushImage<TYPE>(data, dims);
    cle::Image image_B = gpu->CreateImage<TYPE>(dims);

    cle::AddImageAndScalarKernel kernel(gpu);
    kernel.SetInput(image_A);
    kernel.SetOutput(image_B);
    kernel.SetScalar(90.0f);
    kernel.Execute();

    std::vector<TYPE> res = gpu->Pull<TYPE>(image_B);
    

    float diff = 0;
    for (int i =0; i< res.size(); i++)
    {
        diff += valid[i] - res[i];
    }
    if (diff > 0)
    {
        std::cout << "test kernel fail - diff = " << diff << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "test kernel pass - diff = " << diff << std::endl;
    }
    }

    {
    using TYPE = int;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->WaitForKernelToFinish();

    std::array<int,3> dims = {10, 1, 1};
    std::vector<TYPE> data (dims[0]*dims[1]*dims[2]); 
    std::vector<TYPE> valid (dims[0]*dims[1]*dims[2]); 
    std::fill (data.begin(),data.end(), 10.0f);
    std::fill (valid.begin(),valid.end(), 100.0f);

    cle::Buffer buff_A = gpu->PushBuffer<TYPE>(data, dims);
    cle::Buffer buff_B = gpu->CreateBuffer<TYPE>(dims);

    cle::AddImageAndScalarKernel kernel(gpu);
    kernel.SetInput(buff_A);
    kernel.SetOutput(buff_B);
    kernel.SetScalar(90.0f);
    kernel.Execute();

    std::vector<TYPE> res = gpu->Pull<TYPE>(buff_B);
    

    float diff = 0;
    for (int i =0; i< res.size(); i++)
    {
        diff += valid[i] - res[i];
    }
    if (diff > 0)
    {
        std::cout << "test kernel fail - diff = " << diff << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "test kernel pass - diff = " << diff << std::endl;
    }
    }

    {
    using TYPE = unsigned int;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->WaitForKernelToFinish();

        std::array<int,3> dims = {10, 1, 1};
    std::vector<TYPE> data (dims[0]*dims[1]*dims[2]); 
    std::vector<TYPE> valid (dims[0]*dims[1]*dims[2]); 
    std::fill (data.begin(),data.end(), 10.0f);
    std::fill (valid.begin(),valid.end(), 100.0f);

    cle::Buffer buff_A = gpu->PushBuffer<TYPE>(data, dims);
    cle::Buffer buff_B = gpu->CreateBuffer<TYPE>(dims);

    cle::AddImageAndScalarKernel kernel(gpu);
    kernel.SetInput(buff_A);
    kernel.SetOutput(buff_B);
    kernel.SetScalar(90.0f);
    kernel.Execute();

    std::vector<TYPE> res = gpu->Pull<TYPE>(buff_B);
    

    float diff = 0;
    for (int i =0; i< res.size(); i++)
    {
        diff += valid[i] - res[i];
    }
    if (diff > 0)
    {
        std::cout << "test kernel fail - diff = " << diff << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "test kernel pass - diff = " << diff << std::endl;
    }
    }

    {
    using TYPE = char;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->WaitForKernelToFinish();

        std::array<int,3> dims = {10, 1, 1};
    std::vector<TYPE> data (dims[0]*dims[1]*dims[2]); 
    std::vector<TYPE> valid (dims[0]*dims[1]*dims[2]); 
    std::fill (data.begin(),data.end(), 10.0f);
    std::fill (valid.begin(),valid.end(), 100.0f);

    cle::Buffer buff_A = gpu->PushBuffer<TYPE>(data, dims);
    cle::Buffer buff_B = gpu->CreateBuffer<TYPE>(dims);

    cle::AddImageAndScalarKernel kernel(gpu);
    kernel.SetInput(buff_A);
    kernel.SetOutput(buff_B);
    kernel.SetScalar(90.0f);
    kernel.Execute();

    std::vector<TYPE> res = gpu->Pull<TYPE>(buff_B);
    

    float diff = 0;
    for (int i =0; i< res.size(); i++)
    {
        diff += valid[i] - res[i];
    }
    if (diff > 0)
    {
        std::cout << "test kernel fail - diff = " << diff << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "test kernel pass - diff = " << diff << std::endl;
    }
    }

    {
    using TYPE = unsigned char;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->WaitForKernelToFinish();

        std::array<int,3> dims = {10, 1, 1};
    std::vector<TYPE> data (dims[0]*dims[1]*dims[2]); 
    std::vector<TYPE> valid (dims[0]*dims[1]*dims[2]); 
    std::fill (data.begin(),data.end(), 10.0f);
    std::fill (valid.begin(),valid.end(), 100.0f);

    cle::Buffer buff_A = gpu->PushBuffer<TYPE>(data, dims);
    cle::Buffer buff_B = gpu->CreateBuffer<TYPE>(dims);

    cle::AddImageAndScalarKernel kernel(gpu);
    kernel.SetInput(buff_A);
    kernel.SetOutput(buff_B);
    kernel.SetScalar(90.0f);
    kernel.Execute();

    std::vector<TYPE> res = gpu->Pull<TYPE>(buff_B);
    

    float diff = 0;
    for (int i =0; i< res.size(); i++)
    {
        diff += valid[i] - res[i];
    }
    if (diff > 0)
    {
        std::cout << "test kernel fail - diff = " << diff << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "test kernel pass - diff = " << diff << std::endl;
    }
    }

    {
    using TYPE = short;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->WaitForKernelToFinish();

    std::array<int,3> dims = {10, 1, 1};
    std::vector<TYPE> data (dims[0]*dims[1]*dims[2]); 
    std::vector<TYPE> valid (dims[0]*dims[1]*dims[2]); 
    std::fill (data.begin(),data.end(), 10.0f);
    std::fill (valid.begin(),valid.end(), 100.0f);

    cle::Buffer buff_A = gpu->PushBuffer<TYPE>(data, dims);
    cle::Buffer buff_B = gpu->CreateBuffer<TYPE>(dims);

    cle::AddImageAndScalarKernel kernel(gpu);
    kernel.SetInput(buff_A);
    kernel.SetOutput(buff_B);
    kernel.SetScalar(90.0f);
    kernel.Execute();

    std::vector<TYPE> res = gpu->Pull<TYPE>(buff_B);
    

    float diff = 0;
    for (int i =0; i< res.size(); i++)
    {
        diff += valid[i] - res[i];
    }
    if (diff > 0)
    {
        std::cout << "test kernel fail - diff = " << diff << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "test kernel pass - diff = " << diff << std::endl;
    }
    }

    {
    using TYPE = unsigned short;
    auto gpu = std::make_shared<cle::GPU>();
    gpu->WaitForKernelToFinish();

    std::array<int,3> dims = {10, 1, 1};
    std::vector<TYPE> data (dims[0]*dims[1]*dims[2]); 
    std::vector<TYPE> valid (dims[0]*dims[1]*dims[2]); 
    std::fill (data.begin(),data.end(), 10.0f);
    std::fill (valid.begin(),valid.end(), 100.0f);

    cle::Buffer buff_A = gpu->PushBuffer<TYPE>(data, dims);
    cle::Buffer buff_B = gpu->CreateBuffer<TYPE>(dims);

    cle::AddImageAndScalarKernel kernel(gpu);
    kernel.SetInput(buff_A);
    kernel.SetOutput(buff_B);
    kernel.SetScalar(90.0f);
    kernel.Execute();

    std::vector<TYPE> res = gpu->Pull<TYPE>(buff_B);
    

    float diff = 0;
    for (int i =0; i< res.size(); i++)
    {
        diff += valid[i] - res[i];
    }
    if (diff > 0)
    {
        std::cout << "test kernel fail - diff = " << diff << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "test kernel pass - diff = " << diff << std::endl;
    }
    }

    return EXIT_SUCCESS;
}