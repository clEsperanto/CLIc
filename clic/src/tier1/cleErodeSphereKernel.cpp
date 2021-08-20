

#include "cleErodeSphereKernel.hpp"

namespace cle
{

void ErodeSphereKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void ErodeSphereKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void ErodeSphereKernel::Execute()
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
