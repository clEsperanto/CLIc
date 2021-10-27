

#include "cleSetKernel.hpp"

namespace cle
{

SetKernel::SetKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "set",
            {"dst", "value"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "_2d", this->m_OclHeader2d});
    this->m_Sources.insert({this->m_KernelName + "_3d", this->m_OclHeader3d});
}

void SetKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void SetKernel::SetValue(float t_x)
{
    this->AddObject(t_x, "value");
}

void SetKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
