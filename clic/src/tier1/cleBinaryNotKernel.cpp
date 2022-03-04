

#include "cleBinaryNotKernel.hpp"

namespace cle
{

BinaryNotKernel::BinaryNotKernel(std::shared_ptr<GPU> t_gpu) :    
    Kernel( t_gpu, 
            "binary_not",
            {"src", "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void BinaryNotKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void BinaryNotKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void BinaryNotKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
