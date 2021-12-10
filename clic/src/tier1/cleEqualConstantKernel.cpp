

#include "cleEqualConstantKernel.hpp"

namespace cle
{

EqualConstantKernel::EqualConstantKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu, 
            "equal_constant",
            {"src", "dst", "scalar"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void EqualConstantKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
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
    this->ManageDimensions();
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
