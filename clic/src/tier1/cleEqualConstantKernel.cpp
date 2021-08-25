

#include "cleEqualConstantKernel.hpp"

namespace cle
{

EqualConstantKernel::EqualConstantKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu, 
            "equal_constant",
            {"src1", "scalar", "dst"}
    )
{
    m_Sources.insert({this->m_KernelName + "_2d", source_2d});
    m_Sources.insert({this->m_KernelName + "_3d", source_3d});
}

void EqualConstantKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src1");
}

void EqualConstantKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void EqualConstantKernel::SetScalar(float x)
{
    this->AddObject(x, "scalar");
}

void EqualConstantKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
