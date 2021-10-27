

#include "cleGreaterKernel.hpp"

namespace cle
{

GreaterKernel::GreaterKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "greater",
            {"src1", "src2", "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "_2d", this->m_OclHeader2d});
    this->m_Sources.insert({this->m_KernelName + "_3d", this->m_OclHeader3d});
}

void GreaterKernel::SetInput1(Object& t_x)
{
    this->AddObject(t_x, "src1");
}

void GreaterKernel::SetInput2(Object& t_x)
{
    this->AddObject(t_x, "src2");
}

void GreaterKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void GreaterKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
