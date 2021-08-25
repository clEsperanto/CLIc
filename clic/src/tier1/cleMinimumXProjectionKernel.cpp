

#include "cleMinimumXProjectionKernel.hpp"

namespace cle
{

MinimumXProjectionKernel::MinimumXProjectionKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "minimum_x_projection",
            {"dst_min", "src"}
    )
{
    m_Sources.insert({this->m_KernelName + "", source});
}
    
void MinimumXProjectionKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void MinimumXProjectionKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst_min");
}

void MinimumXProjectionKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
