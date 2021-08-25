

#include "cleMinimumZProjectionKernel.hpp"

namespace cle
{

MinimumZProjectionKernel::MinimumZProjectionKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "minimum_z_projection",
            {"dst_min", "src"}
    )
{
    m_Sources.insert({this->m_KernelName + "", source});
}

void MinimumZProjectionKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void MinimumZProjectionKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst_min");
}

void MinimumZProjectionKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
