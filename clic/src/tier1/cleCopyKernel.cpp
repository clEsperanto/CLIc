
#include "cleCopyKernel.hpp"

namespace cle
{

CopyKernel::CopyKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu, 
            "copy",
            {"dst" , "src"}
    )
{
    m_Sources.insert({this->m_KernelName + "_2d", source_2d});
    m_Sources.insert({this->m_KernelName + "_3d", source_3d});
}

void CopyKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void CopyKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void CopyKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
