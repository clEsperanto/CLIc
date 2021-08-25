

#include "cleSumXProjectionKernel.hpp"

namespace cle
{
    
SumXProjectionKernel::SumXProjectionKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "sum_x_projection",
            {"dst", "src"}
    )
{
    m_Sources.insert({this->m_KernelName + "", source});
}

void SumXProjectionKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void SumXProjectionKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void SumXProjectionKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
