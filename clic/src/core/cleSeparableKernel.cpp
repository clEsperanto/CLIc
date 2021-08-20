
#include "cleSeparableKernel.hpp"

namespace cle
{

void SeparableKernel::SetKernelName(std::string name)
{
    this->m_KernelName = name;
}

void SeparableKernel::SetSources(std::map<std::string, std::string> sources)
{
    m_Sources = sources;
}

void SeparableKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void SeparableKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void SeparableKernel::SetSize(int x)
{
    this->AddObject(x, "N");
}

void SeparableKernel::SetSigma(float x)
{
    this->AddObject(x, "s");
}

void SeparableKernel::SetDimension(int x)
{
    this->AddObject(x, "dim");
}

void SeparableKernel::Execute()
{
    if( this->m_Sources.size() > 1)
    {
        this->ManageDimensions("dst");
    }
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
