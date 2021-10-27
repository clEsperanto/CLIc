

#include "cleMinimumZProjectionKernel.hpp"

namespace cle
{

MinimumZProjectionKernel::MinimumZProjectionKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "minimum_z_projection",
            {"dst_min", "src"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "", this->m_OclHeader});
}

void MinimumZProjectionKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void MinimumZProjectionKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst_min");
}

void MinimumZProjectionKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
