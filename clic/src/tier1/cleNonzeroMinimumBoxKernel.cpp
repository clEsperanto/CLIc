

#include "cleNonzeroMinimumBoxKernel.hpp"

namespace cle
{

NonzeroMinimumBoxKernel::NonzeroMinimumBoxKernel (std::shared_ptr<GPU> gpu) : 
    Kernel(gpu,
        "nonzero_minimum_box",
        {"dst", "flag_dst", "src"}
    )
{
    m_Sources.insert({this->m_KernelName + "_2d", source_2d});
    m_Sources.insert({this->m_KernelName + "_3d", source_3d});
}

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
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
