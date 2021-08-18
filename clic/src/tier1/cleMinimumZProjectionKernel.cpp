

#include "cleMinimumZProjectionKernel.hpp"

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
    if(this->m_Sources.size() > 1)
    {
        this->ManageDimensions("dst");
    }
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
