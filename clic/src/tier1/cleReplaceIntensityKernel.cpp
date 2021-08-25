

#include "cleReplaceIntensityKernel.hpp"

namespace cle
{

ReplaceIntensityKernel::ReplaceIntensityKernel (std::shared_ptr<GPU> gpu) : 
    Kernel(gpu,
        "replace_intensity",
        {"dst", "src", "in", "out"}
    )
{
    m_Sources.insert({this->m_KernelName + "", source});
}

void ReplaceIntensityKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void ReplaceIntensityKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void ReplaceIntensityKernel::SetInValue(float x)
{
    this->AddObject(x, "in");
}

void ReplaceIntensityKernel::SetOutValue(float x)
{
    this->AddObject(x, "out");
}

void ReplaceIntensityKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
