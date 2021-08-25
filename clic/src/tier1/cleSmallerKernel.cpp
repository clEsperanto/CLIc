

#include "cleSmallerKernel.hpp"

namespace cle
{

SmallerKernel::SmallerKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu, 
            "smaller",
            {"src1" , "src2", "dst"}
    )
{
    m_Sources.insert({this->m_KernelName + "_2d", source_2d});
    m_Sources.insert({this->m_KernelName + "_3d", source_3d});
}

void SmallerKernel::SetInput1(Buffer& x)
{
    this->AddObject(x, "src1");
}

void SmallerKernel::SetInput2(Buffer& x)
{
    this->AddObject(x, "src2");
}

void SmallerKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void SmallerKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
