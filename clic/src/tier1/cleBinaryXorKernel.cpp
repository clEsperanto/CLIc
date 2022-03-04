

#include "cleBinaryXorKernel.hpp"

namespace cle
{

BinaryXorKernel::BinaryXorKernel(std::shared_ptr<GPU> t_gpu) :    
    Kernel( t_gpu, 
            "binary_xor", 
            {"src0", "src1" , "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void BinaryXorKernel::SetInput1(Object& t_x)
{
    this->AddObject(t_x, "src0");
}

void BinaryXorKernel::SetInput2(Object& t_x)
{
    this->AddObject(t_x, "src1");
}

void BinaryXorKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void BinaryXorKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
