

#include "cleGreaterConstantKernel.hpp"

namespace cle
{

GreaterConstantKernel::GreaterConstantKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "greater_constant",
            {"src1", "scalar", "dst"}
    )
{
    m_Sources.insert({this->m_KernelName + "_2d", source_2d});
    m_Sources.insert({this->m_KernelName + "_3d", source_3d});
}    

void GreaterConstantKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src1");
}

void GreaterConstantKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void GreaterConstantKernel::SetScalar(float x)
{
    this->AddObject(x, "scalar");
}

void GreaterConstantKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
