

#include "cleNotEqualConstantKernel.h"

namespace cle
{

void NotEqualConstantKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src1");
}

void NotEqualConstantKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void NotEqualConstantKernel::SetScalar(float x)
{
    this->AddObject(x, "scalar");
}

void NotEqualConstantKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
