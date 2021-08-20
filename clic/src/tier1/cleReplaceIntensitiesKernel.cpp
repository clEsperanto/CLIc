

#include "cleReplaceIntensitiesKernel.hpp"

namespace cle
{

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
    if(this->m_Sources.size() > 1)
    {
        this->ManageDimensions("dst");
    }
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
