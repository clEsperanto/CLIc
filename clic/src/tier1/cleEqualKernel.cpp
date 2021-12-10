

#include "cleEqualKernel.hpp"

namespace cle
{

EqualKernel::EqualKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "equal",
            {"src0", "src1", "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void EqualKernel::SetInput1(Object& t_x)
{
    this->AddObject(t_x, "src0");
}

void EqualKernel::SetInput2(Object& t_x)
{
    this->AddObject(t_x, "src1");
}

void EqualKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void EqualKernel::Execute()
{
    this->ManageDimensions();
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
