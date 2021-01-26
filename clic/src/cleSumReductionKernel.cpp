

#include "cleSumReductionXKernel.h"

namespace cle
{

void SumReductionXKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void SumReductionXKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void SumReductionXKernel::SetBlocksize(int x)
{
    this->AddObject(x, "blocksize");
}

void SumReductionXKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
