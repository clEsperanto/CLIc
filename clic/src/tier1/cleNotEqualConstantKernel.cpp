

#include "cleNotEqualConstantKernel.hpp"

namespace cle
{

NotEqualConstantKernel::NotEqualConstantKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "not_equal_constant",
            {"src1", "scalar", "dst"}
    )
{
    m_Sources.insert({this->m_KernelName + "_2d", source_2d});
    m_Sources.insert({this->m_KernelName + "_3d", source_3d});
}    

void NotEqualConstantKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src1");
}

void NotEqualConstantKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void NotEqualConstantKernel::SetScalar(float x)
{
    this->AddObject(x, "scalar");
}

void NotEqualConstantKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
