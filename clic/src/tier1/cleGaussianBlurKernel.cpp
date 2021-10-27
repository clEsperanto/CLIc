
#include "cleGaussianBlurKernel.hpp"
#include "cleExecuteSeparableKernel.hpp"

namespace cle
{

GaussianBlurKernel::GaussianBlurKernel (std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "gaussian_blur_separable",
            {"dst", "src"}
        )
{
    this->m_Sources.insert({this->m_KernelName + "_2d", this->m_OclHeader2d});
    this->m_Sources.insert({this->m_KernelName + "_3d", this->m_OclHeader3d});
}    

int GaussianBlurKernel::Sigma2KernelSize(float x) const
{
    int n = static_cast<int>(x * 8.0 + 0.5);
    if (n % 2 == 0)
    {
        n = n + 1;
    }
    return n;
}

void GaussianBlurKernel::SetInput(Buffer& t_x)
{
    this->AddObject(t_x, "src");
}

void GaussianBlurKernel::SetOutput(Buffer& t_x)
{
    this->AddObject(t_x, "dst");
}

void GaussianBlurKernel::SetSigma(float t_x, float t_y, float t_z)
{
    this->m_x = t_x;
    this->m_y = t_y;
    this->m_z = t_z;
}

void GaussianBlurKernel::Execute()
{
    std::shared_ptr<Buffer> src = std::dynamic_pointer_cast<Buffer>(this->m_Parameters.at("src"));
    std::shared_ptr<Buffer> dst = std::dynamic_pointer_cast<Buffer>(this->m_Parameters.at("dst"));

    int nx = Sigma2KernelSize(this->m_x);
    int ny = Sigma2KernelSize(this->m_y);
    int nz = Sigma2KernelSize(this->m_z);

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
