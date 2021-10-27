

#include "cleAbsoluteKernel.hpp"

namespace cle
{

AbsoluteKernel::AbsoluteKernel(std::shared_ptr<GPU> t_gpu) :    
    Kernel( t_gpu, 
            "absolute",     // kernel name
            {"src" , "dst"} // parameter tags
    )
{
    this->m_Sources.insert({this->m_KernelName + "_2d", this->m_OclHeader2d});
    this->m_Sources.insert({this->m_KernelName + "_3d", this->m_OclHeader3d});
}

void AbsoluteKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void AbsoluteKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void AbsoluteKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
