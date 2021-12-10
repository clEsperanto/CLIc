

#include "cleSetColumnKernel.hpp"

namespace cle
{

SetColumnKernel::SetColumnKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "set_column",
            {"dst" , "index", "scalar"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void SetColumnKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void SetColumnKernel::SetColumn(int t_x)
{
    this->AddObject(t_x, "index");
}

void SetColumnKernel::SetValue(float t_x)
{
    this->AddObject(t_x, "scalar");
}

void SetColumnKernel::Execute()
{
    this->ManageDimensions();
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
