

#include "cleNotEqualConstantKernel.hpp"

namespace cle
{

NotEqualConstantKernel::NotEqualConstantKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "not_equal_constant",
            {"src", "dst", "scalar"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}    

void NotEqualConstantKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void NotEqualConstantKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void NotEqualConstantKernel::SetScalar(float t_x)
{
    this->AddObject(t_x, "scalar");
}

void NotEqualConstantKernel::Execute()
{
    this->ManageDimensions();
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
