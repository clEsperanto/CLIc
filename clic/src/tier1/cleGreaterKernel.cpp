

#include "cleGreaterKernel.hpp"

namespace cle
{

GreaterKernel::GreaterKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "greater",
            {"src1", "src2", "dst"}
    )
{
    m_Sources.insert({this->m_KernelName + "_2d", source_2d});
    m_Sources.insert({this->m_KernelName + "_3d", source_3d});
}

void GreaterKernel::SetInput1(Buffer& x)
{
    this->AddObject(x, "src1");
}

void GreaterKernel::SetInput2(Buffer& x)
{
    this->AddObject(x, "src2");
}

void GreaterKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void GreaterKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
