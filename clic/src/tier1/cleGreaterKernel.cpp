

#include "cleGreaterKernel.hpp"

namespace cle
{

GreaterKernel::GreaterKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "greater",
            {"src0", "src1", "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void GreaterKernel::SetInput1(Object& t_x)
{
    this->AddObject(t_x, "src0");
}

void GreaterKernel::SetInput2(Object& t_x)
{
    this->AddObject(t_x, "src1");
}

void GreaterKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void GreaterKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
