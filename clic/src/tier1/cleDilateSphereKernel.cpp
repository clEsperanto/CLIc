

#include "cleDilateSphereKernel.hpp"

namespace cle
{

DilateSphereKernel::DilateSphereKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "dilate_sphere",
            {"src" , "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "_2d", this->m_OclHeader2d});
    this->m_Sources.insert({this->m_KernelName + "_3d", this->m_OclHeader3d});
}

void DilateSphereKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void DilateSphereKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void DilateSphereKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
