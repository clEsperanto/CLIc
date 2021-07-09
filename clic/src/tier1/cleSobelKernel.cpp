

#include "cleSobelKernel.h"

namespace cle
{

void SobelKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void SobelKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}
   
void SobelKernel::Execute()
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
