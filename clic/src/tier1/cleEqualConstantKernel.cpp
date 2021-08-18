

#include "cleEqualConstantKernel.hpp"

namespace cle
{

void EqualConstantKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src1");
}

void EqualConstantKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void EqualConstantKernel::SetScalar(float x)
{
    this->AddObject(x, "scalar");
}

void EqualConstantKernel::Execute()
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
