

#include "cleMinimumZProjectionKernel.hpp"

namespace cle
{

MinimumZProjectionKernel::MinimumZProjectionKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "minimum_z_projection",
            {"src", "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void MinimumZProjectionKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void MinimumZProjectionKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void MinimumZProjectionKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
