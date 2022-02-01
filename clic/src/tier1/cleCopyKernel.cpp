
#include "cleCopyKernel.hpp"

namespace cle
{

CopyKernel::CopyKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu, 
            "copy",
            {"src" , "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
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
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
