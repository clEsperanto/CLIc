
#include "cleMeanSphereKernel.h"

namespace cle
{
    
int MeanSphereKernel::Radius2KernelSize(float r)
{
    return int(r) * 2 + 1;
}

void MeanSphereKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void MeanSphereKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void MeanSphereKernel::SetRadiusX(int x)
{
    int s = Radius2KernelSize(x);
    this->AddObject(s, "radius_x");
}

void MeanSphereKernel::SetRadiusY(int x)
{
    int s = Radius2KernelSize(x);
    this->AddObject(s, "radius_y");
}

void MeanSphereKernel::SetRadiusZ(int x)
{
    int s = Radius2KernelSize(x);
    this->AddObject(s, "radius_z");
}

void MeanSphereKernel::Execute()
{
    if(this->m_Sources.size() > 1)
    {
        this->ManageDimensions("dst");
    }
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
