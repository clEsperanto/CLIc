

#include "cleMinimumYProjectionKernel.h"

namespace cle
{
    
void MinimumYProjectionKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void MinimumYProjectionKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst_min");
}

void MinimumYProjectionKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
