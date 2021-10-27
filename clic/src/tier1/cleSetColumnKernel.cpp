

#include "cleSetColumnKernel.hpp"

namespace cle
{

SetColumnKernel::SetColumnKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "set_column",
            {"dst" , "column", "value"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "_2d", this->m_OclHeader2d});
    this->m_Sources.insert({this->m_KernelName + "_3d", this->m_OclHeader3d});
}

void SetColumnKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void SetColumnKernel::SetColumn(int t_x)
{
    this->AddObject(t_x, "column");
}

void SetColumnKernel::SetValue(float t_x)
{
    this->AddObject(t_x, "value");
}

void SetColumnKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
