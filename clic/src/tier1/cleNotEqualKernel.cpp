

#include "cleNotEqualKernel.hpp"

namespace cle
{


NotEqualKernel::NotEqualKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "not_equal",
            {"src1", "src2", "dst"}
    )
{
    m_Sources.insert({this->m_KernelName + "_2d", source_2d});
    m_Sources.insert({this->m_KernelName + "_3d", source_3d});
}    

void NotEqualKernel::SetInput1(Buffer& x)
{
    this->AddObject(x, "src1");
}

void NotEqualKernel::SetInput2(Buffer& x)
{
    this->AddObject(x, "src2");
}

void NotEqualKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void NotEqualKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
