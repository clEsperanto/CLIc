

#include "cleSetKernel.hpp"

namespace cle
{

SetKernel::SetKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "set",
            {"dst", "value"}
    )
{
    m_Sources.insert({this->m_KernelName + "_2d", source_2d});
    m_Sources.insert({this->m_KernelName + "_3d", source_3d});
}

void SetKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void SetKernel::SetValue(float x)
{
    this->AddObject(x, "value");
}

void SetKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
