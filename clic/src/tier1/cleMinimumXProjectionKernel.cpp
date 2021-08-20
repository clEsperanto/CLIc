

#include "cleMinimumXProjectionKernel.hpp"

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
    if(this->m_Sources.size() > 1)
    {
        this->ManageDimensions("dst");
    }
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
