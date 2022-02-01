

#include "cleNonzeroMinimumBoxKernel.hpp"

namespace cle
{

NonzeroMinimumBoxKernel::NonzeroMinimumBoxKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel(t_gpu,
        "nonzero_minimum_box",
        {"src", "dst0", "dst1"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void NonzeroMinimumBoxKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void NonzeroMinimumBoxKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst1");
}

void NonzeroMinimumBoxKernel::SetOutputFlag(Object& t_x)
{
    this->AddObject(t_x, "dst0");
}

void NonzeroMinimumBoxKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->SetGlobalNDRange("dst1");
    this->EnqueueKernel();
}
} // namespace cle
