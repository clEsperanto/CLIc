

#include "cleNonzeroMinimumBoxKernel.h"

namespace cle
{

void NonzeroMinimumBoxKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void NonzeroMinimumBoxKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void NonzeroMinimumBoxKernel::SetOutputFlag(Buffer& x)
{
    this->AddObject(x, "flag_dst");
}

void NonzeroMinimumBoxKernel::Execute()
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
