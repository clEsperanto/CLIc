

#include "cleGreaterOrEqualConstantKernel.hpp"

namespace cle
{

GreaterOrEqualConstantKernel::GreaterOrEqualConstantKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "greater_or_equal_constant",
            {"src1", "scalar", "dst"}
    )
{
    m_Sources.insert({this->m_KernelName + "_2d", source_2d});
    m_Sources.insert({this->m_KernelName + "_3d", source_3d});
}

void GreaterOrEqualConstantKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src1");
}

void GreaterOrEqualConstantKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void GreaterOrEqualConstantKernel::SetScalar(float x)
{
    this->AddObject(x, "scalar");
}

void GreaterOrEqualConstantKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
