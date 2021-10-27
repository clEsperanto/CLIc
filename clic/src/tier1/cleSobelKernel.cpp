

#include "cleSobelKernel.hpp"

namespace cle
{

SobelKernel::SobelKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "sobel",
            {"dst" , "src"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "_2d", this->m_OclHeader2d});
    this->m_Sources.insert({this->m_KernelName + "_3d", this->m_OclHeader3d});
}

void SobelKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void SobelKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}
   
void SobelKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
