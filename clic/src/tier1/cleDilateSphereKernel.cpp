

#include "cleDilateSphereKernel.hpp"

namespace cle
{

DilateSphereKernel::DilateSphereKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "dilate_sphere",
            {"src" , "dst"}
    )
{
    m_Sources.insert({this->m_KernelName + "_2d", source_2d});
    m_Sources.insert({this->m_KernelName + "_3d", source_3d});
}

void DilateSphereKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void DilateSphereKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void DilateSphereKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
