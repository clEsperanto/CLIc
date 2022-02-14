

#include "cleGreaterConstantKernel.hpp"

namespace cle
{

GreaterConstantKernel::GreaterConstantKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "greater_constant",
            {"src", "dst", "scalar"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}    

void GreaterConstantKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void GreaterConstantKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void GreaterConstantKernel::SetScalar(float t_x)
{
    this->AddObject(t_x, "scalar");
}

void GreaterConstantKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
