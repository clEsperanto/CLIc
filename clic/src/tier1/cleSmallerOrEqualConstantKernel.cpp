

#include "cleSmallerOrEqualConstantKernel.hpp"

namespace cle
{

SmallerOrEqualConstantKernel::SmallerOrEqualConstantKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel(t_gpu, 
        "smaller_or_equal_constant",
        {"src1" , "scalar", "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "_2d", this->m_OclHeader2d});
    this->m_Sources.insert({this->m_KernelName + "_3d", this->m_OclHeader3d});
}    

void SmallerOrEqualConstantKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src1");
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
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
