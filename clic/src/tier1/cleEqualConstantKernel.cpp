

#include "cleEqualConstantKernel.hpp"

namespace cle
{

EqualConstantKernel::EqualConstantKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu, 
            "equal_constant",
            {"src1", "scalar", "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "_2d", this->m_OclHeader2d});
    this->m_Sources.insert({this->m_KernelName + "_3d", this->m_OclHeader3d});
}

void EqualConstantKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src1");
}

void EqualConstantKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void EqualConstantKernel::SetScalar(float t_x)
{
    this->AddObject(t_x, "scalar");
}

void EqualConstantKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
