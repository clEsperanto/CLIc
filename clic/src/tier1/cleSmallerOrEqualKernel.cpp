

#include "cleSmallerOrEqualKernel.hpp"

namespace cle
{

SmallerOrEqualKernel::SmallerOrEqualKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "smaller_or_equal",
            {"src1" , "src2", "dst"}
    )
{
    m_Sources.insert({this->m_KernelName + "_2d", source_2d});
    m_Sources.insert({this->m_KernelName + "_3d", source_3d});
}    

void SmallerOrEqualKernel::SetInput1(Buffer& x)
{
    this->AddObject(x, "src1");
}

void SmallerOrEqualKernel::SetInput2(Buffer& x)
{
    this->AddObject(x, "src2");
}

void SmallerOrEqualKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void SmallerOrEqualKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
