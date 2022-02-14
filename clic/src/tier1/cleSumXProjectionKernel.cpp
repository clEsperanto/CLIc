

#include "cleSumXProjectionKernel.hpp"

namespace cle
{
    
SumXProjectionKernel::SumXProjectionKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "sum_x_projection",
            {"src", "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void SumXProjectionKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void SumXProjectionKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void SumXProjectionKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
