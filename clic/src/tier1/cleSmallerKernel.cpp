

#include "cleSmallerKernel.hpp"

namespace cle
{

SmallerKernel::SmallerKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu, 
            "smaller",
            {"src0" , "src1", "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void SmallerKernel::SetInput1(Object& t_x)
{
    this->AddObject(t_x, "src0");
}

void SmallerKernel::SetInput2(Object& t_x)
{
    this->AddObject(t_x, "src1");
}

void SmallerKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void SmallerKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
