

#include "cleSmallerOrEqualConstantKernel.hpp"

namespace cle
{

SmallerOrEqualConstantKernel::SmallerOrEqualConstantKernel (std::shared_ptr<GPU> gpu) : 
    Kernel(gpu, 
        "smaller_or_equal_constant",
        {"src1" , "scalar", "dst"}
    )
{
    m_Sources.insert({this->m_KernelName + "_2d", source_2d});
    m_Sources.insert({this->m_KernelName + "_3d", source_3d});
}    

void SmallerOrEqualConstantKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src1");
}

void SmallerOrEqualConstantKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void SmallerOrEqualConstantKernel::SetConstant(float x)
{
    this->AddObject(x, "scalar");
}

void SmallerOrEqualConstantKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
