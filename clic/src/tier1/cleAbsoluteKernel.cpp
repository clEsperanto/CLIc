

#include "cleAbsoluteKernel.hpp"

namespace cle
{

AbsoluteKernel::AbsoluteKernel(std::shared_ptr<GPU> t_gpu) :    
    Kernel( t_gpu, 
            "absolute", 
            {"src" , "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
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
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
