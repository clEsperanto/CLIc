
#include "cleAddImageAndScalarKernel.hpp"

namespace cle
{

AddImageAndScalarKernel::AddImageAndScalarKernel(std::shared_ptr<GPU> gpu) : 
    Kernel( gpu, 
            "add_image_and_scalar", 
            {"src", "dst", "scalar"}
        ) 
{
    m_Sources.insert({this->m_KernelName + "_2d", source_2d});
    m_Sources.insert({this->m_KernelName + "_3d", source_3d});
}

void AddImageAndScalarKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void AddImageAndScalarKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void AddImageAndScalarKernel::SetInput(Image2D& x)
{
    this->AddObject(x, "src");
}

void AddImageAndScalarKernel::SetOutput(Image2D& x)
{
    this->AddObject(x, "dst");
}

void AddImageAndScalarKernel::SetScalar(float x)
{
    this->AddObject(x, "scalar");
}

void AddImageAndScalarKernel::Execute()
{
    std::cout << "dim" << std::endl;
    this->ManageDimensions("dst");
    std::cout << "build" << std::endl;
    this->BuildProgramKernel();
    std::cout << "arg" << std::endl;
    this->SetArguments();
    std::cout << "enqueue" << std::endl;
    this->EnqueueKernel();
    std::cout << "done" << std::endl;
}
} // namespace cle

