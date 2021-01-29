

#include "cleGreaterOrEqualKernel.h"

namespace cle
{

void GreaterOrEqualKernel::SetInput1(Buffer& x)
{
    this->AddObject(x, "src1");
}

void GreaterOrEqualKernel::SetInput2(Buffer& x)
{
    this->AddObject(x, "src2");
}

void GreaterOrEqualKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void GreaterOrEqualKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
