

#include "cleBinarySubtractKernel.hpp"

namespace cle
{

BinarySubtractKernel::BinarySubtractKernel(std::shared_ptr<GPU> t_gpu) :    
    Kernel( t_gpu, 
            "binary_subtract", 
            {"src0", "src1" , "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void BinarySubtractKernel::SetInput1(Object& t_x)
{
    this->AddObject(t_x, "src0");
}

void BinarySubtractKernel::SetInput2(Object& t_x)
{
    this->AddObject(t_x, "src1");
}

void BinarySubtractKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void BinarySubtractKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
