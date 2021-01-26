

#include "cleDilateSphereKernel.h"

namespace cle
{

void DilateSphereKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void DilateSphereKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void DilateSphereKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
