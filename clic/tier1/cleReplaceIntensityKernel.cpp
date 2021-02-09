

#include "cleReplaceIntensityKernel.h"

namespace cle
{

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
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
