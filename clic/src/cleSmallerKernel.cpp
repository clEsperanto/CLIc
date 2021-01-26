

#include "cleSmallerKernel.h"

namespace cle
{

void SmallerKernel::SetInput1(Buffer& x)
{
    this->AddObject(x, "src1");
}

void SmallerKernel::SetInput2(Buffer& x)
{
    this->AddObject(x, "src2");
}

void SmallerKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void SmallerKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
