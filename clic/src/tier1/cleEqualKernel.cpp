

#include "cleEqualKernel.hpp"

namespace cle
{

EqualKernel::EqualKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "equal",
            {"src1", "src2", "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "_2d", this->m_OclHeader2d});
    this->m_Sources.insert({this->m_KernelName + "_3d", this->m_OclHeader3d});
}

void EqualKernel::SetInput1(Object& t_x)
{
    this->AddObject(t_x, "src1");
}

void EqualKernel::SetInput2(Object& t_x)
{
    this->AddObject(t_x, "src2");
}

void EqualKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void EqualKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
