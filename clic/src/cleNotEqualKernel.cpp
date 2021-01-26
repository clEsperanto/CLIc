

#include "cleNotEqualKernel.h"

namespace cle
{

void NotEqualKernel::SetInput1(Buffer& x)
{
    this->AddObject(x, "src1");
}

void NotEqualKernel::SetInput2(Buffer& x)
{
    this->AddObject(x, "src2");
}

void NotEqualKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void NotEqualKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
