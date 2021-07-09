
#include "cleMaximumXProjectionKernel.h"

namespace cle
{
    
void MaximumXProjectionKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void MaximumXProjectionKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst_max");
}

void MaximumXProjectionKernel::Execute()
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
