
#include "cleMaximumYProjectionKernel.hpp"

namespace cle
{
    
void MaximumYProjectionKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void MaximumYProjectionKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst_max");
}

void MaximumYProjectionKernel::Execute()
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
