

#include "cleDivideImagesKernel.hpp"

namespace cle
{

DivideImagesKernel::DivideImagesKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,  
            "divide_images",
            {"src0", "src1", "dst"}
        )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void DivideImagesKernel::SetInput1(Object& t_x)
{
    this->AddObject(t_x, "src0");
}

void DivideImagesKernel::SetInput2(Object& t_x)
{
    this->AddObject(t_x, "src1");
}

void DivideImagesKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void DivideImagesKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
