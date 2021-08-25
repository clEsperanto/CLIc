

#include "cleAddImagesWeightedKernel.hpp"

namespace cle
{

AddImagesWeightedKernel::AddImagesWeightedKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,  
            "add_images_weighted",
            {"src", "src1", "dst", "factor", "factor1"}
        )
{
    m_Sources.insert({this->m_KernelName + "_2d", source_2d});
    m_Sources.insert({this->m_KernelName + "_3d", source_3d});
}

void AddImagesWeightedKernel::SetInput1(Buffer& x)
{
    this->AddObject(x, "src");
}

void AddImagesWeightedKernel::SetInput2(Buffer& x)
{
    this->AddObject(x, "src1");
}

void AddImagesWeightedKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void AddImagesWeightedKernel::SetFactor1(float x)
{
    this->AddObject(x, "factor");
}

void AddImagesWeightedKernel::SetFactor2(float x)
{
    this->AddObject(x, "factor1");
}

void AddImagesWeightedKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
