

#include "cleSmallerConstantKernel.hpp"

namespace cle
{

SmallerConstantKernel::SmallerConstantKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "smaller_constant",
            {"src" , "dst", "scalar"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void SmallerConstantKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void SmallerConstantKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void SmallerConstantKernel::SetConstant(float t_x)
{
    this->AddObject(t_x, "scalar");
}

void SmallerConstantKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
