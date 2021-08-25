
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

void AddImageAndScalarKernel::SetScalar(float x)
{
    this->AddObject(x, "scalar");
}

void AddImageAndScalarKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle

