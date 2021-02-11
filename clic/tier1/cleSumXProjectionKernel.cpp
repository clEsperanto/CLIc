

#include "cleSumXProjectionKernel.h"

namespace cle
{
    
void SumXProjectionKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void SumXProjectionKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void SumXProjectionKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
