

#include "cleAbsoluteKernel.h"

namespace cle
{

void AbsoluteKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void AbsoluteKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void AbsoluteKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
