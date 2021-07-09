

#include "cleSumYProjectionKernel.h"

namespace cle
{
    
void SumYProjectionKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void SumYProjectionKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void SumYProjectionKernel::Execute()
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
