

#include "cleSmallerOrEqualKernel.h"

namespace cle
{

void SmallerOrEqualKernel::SetInput1(Buffer& x)
{
    this->AddObject(x, "src1");
}

void SmallerOrEqualKernel::SetInput2(Buffer& x)
{
    this->AddObject(x, "src2");
}

void SmallerOrEqualKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void SmallerOrEqualKernel::Execute()
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
