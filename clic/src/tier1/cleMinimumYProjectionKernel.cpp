

#include "cleMinimumYProjectionKernel.hpp"

namespace cle
{
    
MinimumYProjectionKernel::MinimumYProjectionKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "minimum_y_projection",
            {"src", "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void MinimumYProjectionKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void MinimumYProjectionKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void MinimumYProjectionKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
