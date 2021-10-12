
#include "cleGaussianBlurKernel.hpp"
#include "cleExecuteSeparableKernel.hpp"

namespace cle
{

GaussianBlurKernel::GaussianBlurKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "gaussian_blur_separable",
            {"dst", "src"}
        )
{
    m_Sources.insert({this->m_KernelName + "_2d", source_2d});
    m_Sources.insert({this->m_KernelName + "_3d", source_3d});
}    

int GaussianBlurKernel::Sigma2KernelSize(float x)
{
    int n = static_cast<int>(x * 8.0 + 0.5);
    if (n % 2 == 0)
    {
        n = n + 1;
    }
    return n;
}

void GaussianBlurKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void GaussianBlurKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void GaussianBlurKernel::SetSigma(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void GaussianBlurKernel::Execute()
{
    std::shared_ptr<Buffer> src = std::dynamic_pointer_cast<Buffer>(m_ParameterList.at("src"));
    std::shared_ptr<Buffer> dst = std::dynamic_pointer_cast<Buffer>(m_ParameterList.at("dst"));

    int nx = Sigma2KernelSize(this->x);
    int ny = Sigma2KernelSize(this->y);
    int nz = Sigma2KernelSize(this->z);

    ExecuteSeparableKernel kernel(this->m_gpu);
    kernel.SetKernelName(this->m_KernelName);
    kernel.SetSources(this->m_Sources);
    kernel.SetInput( *src );
    kernel.SetOutput( *dst );
    kernel.SetSigma(this->x, this->y, this->z);
    kernel.SetKernelSize(nx, ny, nz);
    kernel.Execute();
}

} // namespace cle
