

#include "cleAddImagesWeightedKernel.h"

namespace cle
{

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
