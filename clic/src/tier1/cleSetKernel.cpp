

#include "cleSetKernel.hpp"

namespace cle
{

SetKernel::SetKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "set",
            {"dst", "scalar"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void SetKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void SetKernel::SetValue(float t_x)
{
    this->AddObject(t_x, "scalar");
}

void SetKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
