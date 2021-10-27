

#include "cleGreaterOrEqualConstantKernel.hpp"

namespace cle
{

GreaterOrEqualConstantKernel::GreaterOrEqualConstantKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "greater_or_equal_constant",
            {"src1", "scalar", "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "_2d", this->m_OclHeader2d});
    this->m_Sources.insert({this->m_KernelName + "_3d", this->m_OclHeader3d});
}

void GreaterOrEqualConstantKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src1");
}

void GreaterOrEqualConstantKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void GreaterOrEqualConstantKernel::SetScalar(float t_x)
{
    this->AddObject(t_x, "scalar");
}

void GreaterOrEqualConstantKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
