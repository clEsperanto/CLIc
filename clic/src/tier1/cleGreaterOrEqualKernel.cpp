

#include "cleGreaterOrEqualKernel.hpp"

namespace cle
{

GreaterOrEqualKernel::GreaterOrEqualKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "greater_or_equal",
            {"src0", "src1", "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void GreaterOrEqualKernel::SetInput1(Object& t_x)
{
    this->AddObject(t_x, "src0");
}

void GreaterOrEqualKernel::SetInput2(Object& t_x)
{
    this->AddObject(t_x, "src1");
}

void GreaterOrEqualKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void GreaterOrEqualKernel::Execute()
{
    this->ManageDimensions();
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
