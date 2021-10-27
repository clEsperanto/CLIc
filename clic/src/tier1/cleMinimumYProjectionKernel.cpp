

#include "cleMinimumYProjectionKernel.hpp"

namespace cle
{
    
MinimumYProjectionKernel::MinimumYProjectionKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "minimum_y_projection",
            {"dst_min", "src"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "", this->m_OclHeader});
}

void MinimumYProjectionKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void MinimumYProjectionKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst_min");
}

void MinimumYProjectionKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
