
#include "clesperanto.hpp"

#include "cleAddImageAndScalarKernel.hpp"

int main(int argc, char** argv)
{
    cle::Clesperanto* cle = new cle::Clesperanto();

    {
    int dims[3] = {16, 1, 1};
    std::vector<float> A (dims[0]*dims[1]*dims[2]); 
    std::vector<float> B (dims[0]*dims[1]*dims[2]); 

    std::fill (A.begin(),A.end(), 10.0f);
    std::fill (B.begin(),B.end(), 1.0f);


    cl::Image1D image = cle::CreateImage1D<float>(dims, cle->GetGPU());
    cle::WriteImage1D<float>(image, A.data(), cle->GetGPU());
    cle::ReadImage1D<float>(image, B.data(), cle->GetGPU());

    float diff = 0;
    for (size_t i=0; i<A.size(); i++)
    {
        diff += A[i] - B[i];
    }
    std::cout << diff << std::endl;
    }

    {
    int dims[3] = {16, 16, 1};
    std::vector<float> A (dims[0]*dims[1]*dims[2]); 
    std::vector<float> B (dims[0]*dims[1]*dims[2]); 

    std::fill (A.begin(),A.end(), 10.0f);
    std::fill (B.begin(),B.end(), 1.0f);


    cl::Image2D image = cle::CreateImage2D<float>(dims, cle->GetGPU());
    cle::WriteImage2D<float>(image, A.data(), cle->GetGPU());
    cle::ReadImage2D<float>(image, B.data(), cle->GetGPU());

    float diff = 0;
    for (size_t i=0; i<A.size(); i++)
    {
        diff += A[i] - B[i];
    }
    std::cout << diff << std::endl;
    }

    {
    int dims[3] = {16, 16, 5};
    std::vector<float> A (dims[0]*dims[1]*dims[2]); 
    std::vector<float> B (dims[0]*dims[1]*dims[2]); 

    std::fill (A.begin(),A.end(), 10.0f);
    std::fill (B.begin(),B.end(), 1.0f);


    cl::Image3D image = cle::CreateImage3D<float>(dims, cle->GetGPU());
    cle::WriteImage3D<float>(image, A.data(), cle->GetGPU());
    cle::ReadImage3D<float>(image, B.data(), cle->GetGPU());

    float diff = 0;
    for (size_t i=0; i<A.size(); i++)
    {
        diff += A[i] - B[i];
    }
    std::cout << diff << std::endl;
    }

    int dims[3] = {16, 16, 1};
    std::vector<float> A (dims[0]*dims[1]*dims[2]); 
    std::vector<float> B (dims[0]*dims[1]*dims[2]); 
    std::vector<float> C (dims[0]*dims[1]*dims[2]); 

    std::fill (A.begin(),A.end(), 1.0f);
    std::fill (B.begin(),B.end(), 0.0f);
    std::fill (C.begin(),C.end(), 101.0f);


    // kernel run using Image2D object
    
    cl::Image2D ocl_imageA = cle::CreateImage2D<float>(dims, cle->GetGPU());
    cle::WriteImage2D<float>(ocl_imageA, A.data(), cle->GetGPU());
    cle::Image2D ImageA (ocl_imageA, dims);

    cl::Image2D ocl_imageB = cle::CreateImage2D<float>(dims, cle->GetGPU());
    cle::Image2D ImageB (ocl_imageB, dims);

    cle::AddImageAndScalarKernel kernel (cle->GetGPU());
    kernel.SetScalar(100.0f);
    kernel.SetInput(ImageA);
    kernel.SetOutput(ImageB);
    kernel.Execute();

    cle::ReadImage2D<float>(ImageB.GetObject(), B.data(), cle->GetGPU());

    float diff = 0;
    for (size_t i=0; i<A.size(); i++)
    {
        diff += B[i] - C[i];
    }

    return diff;
}