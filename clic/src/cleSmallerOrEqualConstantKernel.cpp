

#include "cleSmallerOrEqualConstantKernel.h"

namespace cle
{

void SmallerOrEqualConstantKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src1");
}

void SmallerOrEqualConstantKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void SmallerOrEqualConstantKernel::SetConstant(float x)
{
    this->AddObject(x, "scalar");
}

void SmallerOrEqualConstantKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
