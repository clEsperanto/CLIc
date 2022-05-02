
#include "cleSubtractImageFromScalarKernel.hpp"

namespace cle
{

SubtractImageFromScalarKernel::SubtractImageFromScalarKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu, 
            "subtract_image_from_scalar", 
            {"src", "dst", "scalar"}
        ) 
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void SubtractImageFromScalarKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void SubtractImageFromScalarKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void SubtractImageFromScalarKernel::SetScalar(float t_x)
{
    this->AddObject(t_x, "scalar");
}

void SubtractImageFromScalarKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle

