
#include "cleAddImageAndScalarKernel.h"

namespace cle
{

void AddImageAndScalarKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void AddImageAndScalarKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void AddImageAndScalarKernel::SetScalar(float x)
{
    this->AddObject(x, "scalar");
}

void AddImageAndScalarKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle

