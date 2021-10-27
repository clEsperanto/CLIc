

#include "cleAddImagesWeightedKernel.hpp"

namespace cle
{

AddImagesWeightedKernel::AddImagesWeightedKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,  
            "add_images_weighted",
            {"src", "src1", "dst", "factor", "factor1"}
        )
{
    this->m_Sources.insert({this->m_KernelName + "_2d", this->m_OclHeader2d});
    this->m_Sources.insert({this->m_KernelName + "_3d", this->m_OclHeader3d});
}

void AddImagesWeightedKernel::SetInput1(Object& t_x)
{
    this->AddObject(t_x, "src");
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
    this->AddObject(t_x, "factor");
}

void AddImagesWeightedKernel::SetFactor2(float t_x)
{
    this->AddObject(t_x, "factor1");
}

void AddImagesWeightedKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
