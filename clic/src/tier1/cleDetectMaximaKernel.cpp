

#include "cleDetectMaximaKernel.hpp"

namespace cle
{

void DetectMaximaKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void DetectMaximaKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void DetectMaximaKernel::Execute()
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
