

#include "cleGreaterOrEqualConstantKernel.hpp"

namespace cle
{

GreaterOrEqualConstantKernel::GreaterOrEqualConstantKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "greater_or_equal_constant",
            {"src", "dst", "scalar"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void GreaterOrEqualConstantKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
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
    this->ManageDimensions();
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
