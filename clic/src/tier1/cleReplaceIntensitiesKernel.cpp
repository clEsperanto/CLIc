

#include "cleReplaceIntensitiesKernel.hpp"

namespace cle
{

ReplaceIntensitiesKernel::ReplaceIntensitiesKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "replace_intensities",
            {"dst", "src", "map"}
    )
{
    m_Sources.insert({this->m_KernelName + "", source});
} 

void ReplaceIntensitiesKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void ReplaceIntensitiesKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void ReplaceIntensitiesKernel::SetMap(Buffer& x)
{
    this->AddObject(x, "map");
}

void ReplaceIntensitiesKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
