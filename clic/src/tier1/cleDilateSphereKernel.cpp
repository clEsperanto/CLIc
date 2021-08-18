

#include "cleDilateSphereKernel.hpp"

namespace cle
{

void DilateSphereKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void DilateSphereKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void DilateSphereKernel::Execute()
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
