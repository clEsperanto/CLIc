

#include "cleReplaceIntensityKernel.hpp"

namespace cle
{

ReplaceIntensityKernel::ReplaceIntensityKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel(t_gpu,
        "replace_intensity",
        {"dst", "src", "in", "out"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "", this->m_OclHeader});
}

void ReplaceIntensityKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void ReplaceIntensityKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void ReplaceIntensityKernel::SetInValue(float t_x)
{
    this->AddObject(t_x, "in");
}

void ReplaceIntensityKernel::SetOutValue(float t_x)
{
    this->AddObject(t_x, "out");
}

void ReplaceIntensityKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
