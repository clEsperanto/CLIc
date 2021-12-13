

#include "cleSobelKernel.hpp"

namespace cle
{

SobelKernel::SobelKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "sobel",
            {"src" , "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
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
    this->ManageDimensions();
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
