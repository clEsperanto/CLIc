

#include "cleMinimumYProjectionKernel.hpp"

namespace cle
{
    
MinimumYProjectionKernel::MinimumYProjectionKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "minimum_y_projection",
            {"dst_min", "src"}
    )
{
    m_Sources.insert({this->m_KernelName + "", source});
}

void MinimumYProjectionKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void MinimumYProjectionKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst_min");
}

void MinimumYProjectionKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
