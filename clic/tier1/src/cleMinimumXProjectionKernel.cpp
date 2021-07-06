

#include "cleMinimumXProjectionKernel.h"

namespace cle
{
    
void MinimumXProjectionKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void MinimumXProjectionKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst_min");
}

void MinimumXProjectionKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
