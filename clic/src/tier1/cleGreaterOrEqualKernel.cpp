

#include "cleGreaterOrEqualKernel.hpp"

namespace cle
{

GreaterOrEqualKernel::GreaterOrEqualKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "greater_or_equal",
            {"src1", "src2", "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "_2d", this->m_OclHeader2d});
    this->m_Sources.insert({this->m_KernelName + "_3d", this->m_OclHeader3d});
}

void GreaterOrEqualKernel::SetInput1(Object& t_x)
{
    this->AddObject(t_x, "src1");
}

void GreaterOrEqualKernel::SetInput2(Object& t_x)
{
    this->AddObject(t_x, "src2");
}

void GreaterOrEqualKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void GreaterOrEqualKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
