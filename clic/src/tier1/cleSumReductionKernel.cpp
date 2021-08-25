

#include "cleSumReductionXKernel.hpp"

namespace cle
{

SumReductionXKernel::SumReductionXKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "sum_reduction_x",
            {"dst", "src", "blocksize"}
    )
{
    m_Sources.insert({this->m_KernelName + "", source});
}

void SumReductionXKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void SumReductionXKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void SumReductionXKernel::SetBlocksize(int x)
{
    this->AddObject(x, "blocksize");
}

void SumReductionXKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
