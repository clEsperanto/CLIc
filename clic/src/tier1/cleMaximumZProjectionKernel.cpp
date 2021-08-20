
#include "cleMaximumZProjectionKernel.hpp"

namespace cle
{
    
void MaximumZProjectionKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void MaximumZProjectionKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst_max");
}

void MaximumZProjectionKernel::Execute()
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
