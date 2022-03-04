

#include "cleBinaryOrKernel.hpp"

namespace cle
{

BinaryOrKernel::BinaryOrKernel(std::shared_ptr<GPU> t_gpu) :    
    Kernel( t_gpu, 
            "binary_or", 
            {"src0", "src1" , "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void BinaryOrKernel::SetInput1(Object& t_x)
{
    this->AddObject(t_x, "src0");
}

void BinaryOrKernel::SetInput2(Object& t_x)
{
    this->AddObject(t_x, "src1");
}

void BinaryOrKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void BinaryOrKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
