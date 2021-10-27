

#include "cleSmallerConstantKernel.hpp"

namespace cle
{

SmallerConstantKernel::SmallerConstantKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "smaller_constant",
            {"src1" , "scalar", "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "_2d", this->m_OclHeader2d});
    this->m_Sources.insert({this->m_KernelName + "_3d", this->m_OclHeader3d});
}

void SmallerConstantKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src1");
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
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
