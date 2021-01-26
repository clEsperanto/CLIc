

#include "cleGreaterKernel.h"

namespace cle
{

void GreaterKernel::SetInput1(Buffer& x)
{
    this->AddObject(x, "src1");
}

void GreaterKernel::SetInput2(Buffer& x)
{
    this->AddObject(x, "src2");
}

void GreaterKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void GreaterKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
