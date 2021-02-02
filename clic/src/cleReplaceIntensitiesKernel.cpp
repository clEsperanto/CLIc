

#include "cleReplaceIntensitiesKernel.h"

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
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
