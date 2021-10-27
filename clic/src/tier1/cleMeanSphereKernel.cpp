
#include "cleMeanSphereKernel.hpp"

namespace cle
{

MeanSphereKernel::MeanSphereKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "mean_sphere",
            {"dst", "src", "radius_x", "radius_y", "radius_z"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "_2d", this->m_OclHeader2d});
    this->m_Sources.insert({this->m_KernelName + "_3d", this->m_OclHeader3d});
}    
    
int MeanSphereKernel::Radius2KernelSize(float t_r) const
{
    return static_cast<int>(t_r) * 2 + 1;
}

void MeanSphereKernel::SetRadius(int t_x, int t_y, int t_z)
{
    int dx = Radius2KernelSize(t_x);
    this->AddObject(dx, "radius_x");
    int dy = Radius2KernelSize(t_y);
    this->AddObject(dy, "radius_y");
    if(t_z > 0)
    {
        int dz = Radius2KernelSize(t_z);
        this->AddObject(dz, "radius_z");
    }
}

void MeanSphereKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void MeanSphereKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void MeanSphereKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
