

#include "cleSmallerConstantKernel.hpp"

namespace cle
{

SmallerConstantKernel::SmallerConstantKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "smaller_constant",
            {"src1" , "scalar", "dst"}
    )
{
    m_Sources.insert({this->m_KernelName + "_2d", source_2d});
    m_Sources.insert({this->m_KernelName + "_3d", source_3d});
}

void SmallerConstantKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src1");
}

void SmallerConstantKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void SmallerConstantKernel::SetConstant(float x)
{
    this->AddObject(x, "scalar");
}

void SmallerConstantKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
