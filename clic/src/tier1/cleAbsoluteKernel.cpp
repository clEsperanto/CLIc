

#include "cleAbsoluteKernel.hpp"

namespace cle
{

void AbsoluteKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void AbsoluteKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void AbsoluteKernel::Execute()
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
