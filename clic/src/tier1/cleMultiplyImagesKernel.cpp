

#include "cleMultiplyImagesKernel.hpp"

namespace cle
{

MultiplyImagesKernel::MultiplyImagesKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,  
            "multiply_images",
            {"src0", "src1", "dst"}
        )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void MultiplyImagesKernel::SetInput1(Object& t_x)
{
    this->AddObject(t_x, "src0");
}

void MultiplyImagesKernel::SetInput2(Object& t_x)
{
    this->AddObject(t_x, "src1");
}

void MultiplyImagesKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void MultiplyImagesKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
