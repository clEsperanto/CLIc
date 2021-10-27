

#include "cleMinimumXProjectionKernel.hpp"

namespace cle
{

MinimumXProjectionKernel::MinimumXProjectionKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "minimum_x_projection",
            {"dst_min", "src"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "", this->m_OclHeader});
}
    
void MinimumXProjectionKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void MinimumXProjectionKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst_min");
}

void MinimumXProjectionKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
