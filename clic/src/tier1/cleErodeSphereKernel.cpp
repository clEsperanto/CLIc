

#include "cleErodeSphereKernel.hpp"

namespace cle
{

ErodeSphereKernel::ErodeSphereKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "erode_sphere",
            {"src" , "dst"}
    )
{
    m_Sources.insert({this->m_KernelName + "_2d", source_2d});
    m_Sources.insert({this->m_KernelName + "_3d", source_3d});
}

void ErodeSphereKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void ErodeSphereKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void ErodeSphereKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
