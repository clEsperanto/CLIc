
#include "cleCopyKernel.hpp"

namespace cle
{

CopyKernel::CopyKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu, 
            "copy",
            {"dst" , "src"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "_2d", this->m_OclHeader2d});
    this->m_Sources.insert({this->m_KernelName + "_3d", this->m_OclHeader3d});
}

void CopyKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void CopyKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void CopyKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
