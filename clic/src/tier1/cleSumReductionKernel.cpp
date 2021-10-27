

#include "cleSumReductionXKernel.hpp"

namespace cle
{

SumReductionXKernel::SumReductionXKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "sum_reduction_x",
            {"dst", "src", "blocksize"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "", this->m_OclHeader});
}

void SumReductionXKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void SumReductionXKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void SumReductionXKernel::SetBlocksize(int t_x)
{
    this->AddObject(t_x, "blocksize");
}

void SumReductionXKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
