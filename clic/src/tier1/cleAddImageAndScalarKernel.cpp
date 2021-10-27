
#include "cleAddImageAndScalarKernel.hpp"

namespace cle
{

AddImageAndScalarKernel::AddImageAndScalarKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu, 
            "add_image_and_scalar", 
            {"src", "dst", "scalar"}
        ) 
{
    this->m_Sources.insert({this->m_KernelName + "_2d", this->m_OclHeader2d});
    this->m_Sources.insert({this->m_KernelName + "_3d", this->m_OclHeader3d});
}

void AddImageAndScalarKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void AddImageAndScalarKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void AddImageAndScalarKernel::SetScalar(float t_x)
{
    this->AddObject(t_x, "scalar");
}

void AddImageAndScalarKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle

