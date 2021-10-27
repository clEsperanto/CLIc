

#include "cleMeanBoxKernel.hpp"
#include "cleExecuteSeparableKernel.hpp"

namespace cle
{

MeanBoxKernel::MeanBoxKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu, 
            "mean_separable",
            {"dst", "src"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "_2d", this->m_OclHeader2d});
    this->m_Sources.insert({this->m_KernelName + "_3d", this->m_OclHeader3d});    
}    

int MeanBoxKernel::Radius2KernelSize(float t_r) const
{
    return static_cast<int>(t_r) * 2 + 1;
}

void MeanBoxKernel::SetInput(Buffer& t_x)
{
    this->AddObject(t_x, "src");
}

void MeanBoxKernel::SetOutput(Buffer& t_x)
{
    this->AddObject(t_x, "dst");
}

void MeanBoxKernel::SetRadius(float t_x, float t_y, float t_z)
{
    this->m_x = t_x;
    this->m_y = t_y;
    this->m_z = t_z;
}

void MeanBoxKernel::Execute()
{
    std::shared_ptr<cle::Buffer> src = std::dynamic_pointer_cast<cle::Buffer>(this->m_Parameters.at("src"));
    std::shared_ptr<cle::Buffer> dst = std::dynamic_pointer_cast<cle::Buffer>(this->m_Parameters.at("dst"));

    int nx = Radius2KernelSize(this->m_x);
    int ny = Radius2KernelSize(this->m_y);
    int nz = Radius2KernelSize(this->m_z);

    ExecuteSeparableKernel kernel(this->m_gpu);
    kernel.SetKernelName(this->m_KernelName);
    kernel.SetSources(this->m_Sources);
    kernel.SetInput( *src );
    kernel.SetOutput( *dst );
    kernel.SetSigma(this->m_x, this->m_y, this->m_z);
    kernel.SetKernelSize(nx, ny, nz);
    kernel.Execute();
}

} // namespace cle
