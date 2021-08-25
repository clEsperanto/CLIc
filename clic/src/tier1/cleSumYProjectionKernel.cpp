

#include "cleSumYProjectionKernel.hpp"

namespace cle
{
   
SumYProjectionKernel::SumYProjectionKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "sum_y_projection",
            {"dst", "src"}
    )
{
    m_Sources.insert({this->m_KernelName + "", source});
}   

void SumYProjectionKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void SumYProjectionKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void SumYProjectionKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
