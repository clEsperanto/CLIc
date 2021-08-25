

#include "cleSumZProjectionKernel.hpp"

namespace cle
{

SumZProjectionKernel::SumZProjectionKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "sum_z_projection",
            {"dst", "src"}
    )
{
    m_Sources.insert({this->m_KernelName + "", source});
}    
    
void SumZProjectionKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void SumZProjectionKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void SumZProjectionKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
