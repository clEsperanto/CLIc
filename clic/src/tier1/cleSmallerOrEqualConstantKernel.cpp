

#include "cleSmallerOrEqualConstantKernel.hpp"

namespace cle
{

SmallerOrEqualConstantKernel::SmallerOrEqualConstantKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel(t_gpu, 
        "smaller_or_equal_constant",
        {"src" , "dst", "scalar"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}    

void SmallerOrEqualConstantKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void SmallerOrEqualConstantKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void SmallerOrEqualConstantKernel::SetConstant(float t_x)
{
    this->AddObject(t_x, "scalar");
}

void SmallerOrEqualConstantKernel::Execute()
{
    this->ManageDimensions();
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
