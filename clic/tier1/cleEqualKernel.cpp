

#include "cleEqualKernel.h"

namespace cle
{

void EqualKernel::SetInput1(Buffer& x)
{
    this->AddObject(x, "src1");
}

void EqualKernel::SetInput2(Buffer& x)
{
    this->AddObject(x, "src2");
}

void EqualKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void EqualKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
