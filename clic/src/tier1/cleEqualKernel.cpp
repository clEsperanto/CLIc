

#include "cleEqualKernel.hpp"

namespace cle
{

EqualKernel::EqualKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "equal",
            {"src1", "src2", "dst"}
    )
{
    m_Sources.insert({this->m_KernelName + "_2d", source_2d});
    m_Sources.insert({this->m_KernelName + "_3d", source_3d});
}

void EqualKernel::SetInput1(Buffer& x)
{
    this->AddObject(x, "src1");
}

void EqualKernel::SetInput2(Buffer& x)
{
    this->AddObject(x, "src2");
}

void EqualKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void EqualKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
