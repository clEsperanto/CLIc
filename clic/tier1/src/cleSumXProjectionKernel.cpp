

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
    if(this->m_Sources.size() > 1)
    {
        this->ManageDimensions("dst");
    }
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
