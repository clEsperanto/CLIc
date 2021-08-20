
#include "cleMeanSphereKernel.hpp"

namespace cle
{
    
int MeanSphereKernel::Radius2KernelSize(float r)
{
    return int(r) * 2 + 1;
}

void MeanSphereKernel::SetRadius(int x, int y, int z)
{
    int dx = Radius2KernelSize(x);
    this->AddObject(dx, "radius_x");
    int dy = Radius2KernelSize(y);
    this->AddObject(dy, "radius_y");
    if(z > 0)
    {
        int dz = Radius2KernelSize(z);
        this->AddObject(dz, "radius_z");
    }
}

void MeanSphereKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void MeanSphereKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
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
