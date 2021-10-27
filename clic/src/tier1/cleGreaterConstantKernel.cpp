

#include "cleGreaterConstantKernel.hpp"

namespace cle
{

GreaterConstantKernel::GreaterConstantKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "greater_constant",
            {"src1", "scalar", "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "_2d", this->m_OclHeader2d});
    this->m_Sources.insert({this->m_KernelName + "_3d", this->m_OclHeader3d});
}    

void GreaterConstantKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src1");
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
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
