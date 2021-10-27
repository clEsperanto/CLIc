

#include "cleSumZProjectionKernel.hpp"

namespace cle
{

SumZProjectionKernel::SumZProjectionKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "sum_z_projection",
            {"dst", "src"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "", this->m_OclHeader});
}    
    
void SumZProjectionKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void SumZProjectionKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void SumZProjectionKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
