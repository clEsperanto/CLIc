

#include "cleBinaryAndKernel.hpp"

namespace cle
{

BinaryAndKernel::BinaryAndKernel(std::shared_ptr<GPU> t_gpu) :    
    Kernel( t_gpu, 
            "binary_and", 
            {"src0", "src1" , "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void BinaryAndKernel::SetInput1(Object& t_x)
{
    this->AddObject(t_x, "src0");
}

void BinaryAndKernel::SetInput2(Object& t_x)
{
    this->AddObject(t_x, "src1");
}

void BinaryAndKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void BinaryAndKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
