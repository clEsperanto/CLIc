
#include "cleMeanSphereKernel.hpp"

namespace cle
{

MeanSphereKernel::MeanSphereKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "mean_sphere",
            {"src", "dst", "scalar0", "scalar1", "scalar2"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}    
    
int MeanSphereKernel::Radius2KernelSize(float t_r) const
{
    return static_cast<int>(t_r) * 2 + 1;
}

void MeanSphereKernel::SetRadius(int t_x, int t_y, int t_z)
{
    int dx = Radius2KernelSize(t_x);
    this->AddObject(dx, "scalar0");
    int dy = Radius2KernelSize(t_y);
    this->AddObject(dy, "scalar1");
    int dz = Radius2KernelSize(t_z);
    this->AddObject(dz, "scalar2");
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
    this->ManageDimensions();
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
