

#include "cleSumYProjectionKernel.hpp"

namespace cle
{
   
SumYProjectionKernel::SumYProjectionKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "sum_y_projection",
            {"dst", "src"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "", this->m_OclHeader});
}   

void SumYProjectionKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void SumYProjectionKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void SumYProjectionKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
