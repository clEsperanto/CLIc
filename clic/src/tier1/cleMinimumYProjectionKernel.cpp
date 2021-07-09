

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
    if(this->m_Sources.size() > 1)
    {
        this->ManageDimensions("dst");
    }
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
