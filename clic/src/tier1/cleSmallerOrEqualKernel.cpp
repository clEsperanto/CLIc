

#include "cleSmallerOrEqualKernel.hpp"

namespace cle
{

SmallerOrEqualKernel::SmallerOrEqualKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "smaller_or_equal",
            {"src0" , "src1", "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}    

void SmallerOrEqualKernel::SetInput1(Object& t_x)
{
    this->AddObject(t_x, "src0");
}

void SmallerOrEqualKernel::SetInput2(Object& t_x)
{
    this->AddObject(t_x, "src1");
}

void SmallerOrEqualKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void SmallerOrEqualKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
