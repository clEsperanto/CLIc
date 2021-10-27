

#include "cleMaximumBoxKernel.hpp"
#include "cleExecuteSeparableKernel.hpp"

namespace cle
{

MaximumBoxKernel::MaximumBoxKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "maximum_separable",
            {"dst", "src"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "_2d", this->m_OclHeader2d});
    this->m_Sources.insert({this->m_KernelName + "_3d", this->m_OclHeader3d}); 
}    

int MaximumBoxKernel::Radius2KernelSize(float t_r) const
{
    return static_cast<int>(t_r) * 2 + 1;
}

void MaximumBoxKernel::SetInput(Buffer& t_x)
{
    this->AddObject(t_x, "src");
}

void MaximumBoxKernel::SetOutput(Buffer& t_x)
{
    this->AddObject(t_x, "dst");
}

void MaximumBoxKernel::SetRadius(float x, float y, float z)
{
    this->m_x = x;
    this->m_y = y;
    this->m_z = z;
}

void MaximumBoxKernel::Execute()
{
    std::shared_ptr<Buffer> src = std::dynamic_pointer_cast<Buffer>(this->m_Parameters.at("src"));
    std::shared_ptr<Buffer> dst = std::dynamic_pointer_cast<Buffer>(this->m_Parameters.at("dst"));

    int nx = Radius2KernelSize(this->m_x);
    int ny = Radius2KernelSize(this->m_y);
    int nz = Radius2KernelSize(this->m_z);

    ExecuteSeparableKernel kernel(this->m_gpu);
    kernel.SetKernelName(this->m_KernelName);
    kernel.SetSources(this->m_Sources);
    kernel.SetInput(*src);
    kernel.SetOutput(*dst);
    kernel.SetSigma(this->m_x, this->m_y, this->m_z);
    kernel.SetKernelSize(nx, ny, nz);
    kernel.Execute();
}

} // namespace cle
