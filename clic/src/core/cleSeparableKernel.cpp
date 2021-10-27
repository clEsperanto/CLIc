
#include "cleSeparableKernel.hpp"

namespace cle
{

SeparableKernel::SeparableKernel (std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu, 
            "",
            {"dst" , "src", "dim", "N", "s"}
    )
{}

void SeparableKernel::SetKernelName(const std::string& t_name)
{
    this->m_KernelName = t_name;
}

void SeparableKernel::SetSources(const std::map<std::string, std::string>& t_sources)
{
    this->m_Sources = t_sources;
}

void SeparableKernel::SetInput(Buffer& t_x)
{
    this->AddObject(t_x, "src");
}

void SeparableKernel::SetOutput(Buffer& t_x)
{
    this->AddObject(t_x, "dst");
}

void SeparableKernel::SetSize(int t_x)
{
    this->AddObject(t_x, "N");
}

void SeparableKernel::SetSigma(float t_x)
{
    this->AddObject(t_x, "s");
}

void SeparableKernel::SetDimension(int t_x)
{
    this->AddObject(t_x, "dim");
}

void SeparableKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
