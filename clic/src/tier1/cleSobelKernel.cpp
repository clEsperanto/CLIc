

#include "cleSobelKernel.hpp"

namespace cle
{

SobelKernel::SobelKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "sobel",
            {"dst" , "src"}
    )
{
    m_Sources.insert({this->m_KernelName + "_2d", source_2d});
    m_Sources.insert({this->m_KernelName + "_3d", source_3d});
}

void SobelKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void SobelKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}
   
void SobelKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
