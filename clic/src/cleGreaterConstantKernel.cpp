

#include "cleGreaterConstantKernel.h"

namespace cle
{

void GreaterConstantKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src1");
}

void GreaterConstantKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void GreaterConstantKernel::SetScalar(float x)
{
    this->AddObject(x, "scalar");
}

void GreaterConstantKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
