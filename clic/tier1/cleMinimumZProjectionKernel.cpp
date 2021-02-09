

#include "cleMinimumZProjectionKernel.h"

namespace cle
{
    
void MinimumZProjectionKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void MinimumZProjectionKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst_min");
}

void MinimumZProjectionKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
