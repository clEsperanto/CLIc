

#include "cleSumZProjectionKernel.h"

namespace cle
{
    
void SumZProjectionKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void SumZProjectionKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void SumZProjectionKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
