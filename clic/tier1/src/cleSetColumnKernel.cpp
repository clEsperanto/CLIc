

#include "cleSetColumnKernel.h"

namespace cle
{

void SetColumnKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void SetColumnKernel::SetColumn(int x)
{
    this->AddObject(x, "column");
}

void SetColumnKernel::SetValue(float x)
{
    this->AddObject(x, "value");
}

void SetColumnKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
