

#include "cleNonzeroMinimumBoxKernel.hpp"

namespace cle
{

NonzeroMinimumBoxKernel::NonzeroMinimumBoxKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel(t_gpu,
        "nonzero_minimum_box",
        {"dst", "flag_dst", "src"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "_2d", this->m_OclHeader2d});
    this->m_Sources.insert({this->m_KernelName + "_3d", this->m_OclHeader3d});
}

void NonzeroMinimumBoxKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void NonzeroMinimumBoxKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void NonzeroMinimumBoxKernel::SetOutputFlag(Object& t_x)
{
    this->AddObject(t_x, "flag_dst");
}

void NonzeroMinimumBoxKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
