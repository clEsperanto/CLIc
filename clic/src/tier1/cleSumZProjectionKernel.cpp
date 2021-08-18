

#include "cleSumZProjectionKernel.hpp"

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
    if(this->m_Sources.size() > 1)
    {
        this->ManageDimensions("dst");
    }
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
