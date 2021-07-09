

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
    if(this->m_Sources.size() > 1)
    {
        this->ManageDimensions("dst");
    }
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
