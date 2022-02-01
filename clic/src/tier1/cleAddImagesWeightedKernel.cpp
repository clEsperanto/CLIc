

#include "cleAddImagesWeightedKernel.hpp"

namespace cle
{

AddImagesWeightedKernel::AddImagesWeightedKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,  
            "add_images_weighted",
            {"src0", "src1", "dst", "scalar0", "scalar1"}
        )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void AddImagesWeightedKernel::SetInput1(Object& t_x)
{
    this->AddObject(t_x, "src0");
}

void AddImagesWeightedKernel::SetInput2(Object& t_x)
{
    this->AddObject(t_x, "src1");
}

void AddImagesWeightedKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void AddImagesWeightedKernel::SetFactor1(float t_x)
{
    this->AddObject(t_x, "scalar0");
}

void AddImagesWeightedKernel::SetFactor2(float t_x)
{
    this->AddObject(t_x, "scalar1");
}

void AddImagesWeightedKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
