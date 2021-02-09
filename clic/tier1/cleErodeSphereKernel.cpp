

#include "cleErodeSphereKernel.h"

namespace cle
{

void ErodeSphereKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void ErodeSphereKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void ErodeSphereKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
