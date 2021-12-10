

#include "cleErodeSphereKernel.hpp"

namespace cle
{

ErodeSphereKernel::ErodeSphereKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "erode_sphere",
            {"src" , "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void ErodeSphereKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void ErodeSphereKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void ErodeSphereKernel::Execute()
{
    this->ManageDimensions();
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
