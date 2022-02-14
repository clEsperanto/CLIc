
#include "cleGaussianBlurKernel.hpp"
#include "cleExecuteSeparableKernel.hpp"

namespace cle
{

GaussianBlurKernel::GaussianBlurKernel (std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "gaussian_blur_separable",
            {"src", "dst"}
        )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}    

std::array<int,3> GaussianBlurKernel::Sigma2KernelSize(std::array<float,3> t_sigma) const
{
    std::array<int,3> ksize = {0,0,0};
    auto it_size = ksize.begin(); 
    auto it_sigma = t_sigma.begin();
    for( ; it_sigma != t_sigma.end(), it_size!= ksize.end(); ++it_sigma, ++it_size)
    {
        *it_size = static_cast<int>(*it_sigma * 8.0 + 0.5);
        if (*it_size % 2 == 0)
        {
            *it_size += 1;
        }
    }
    return ksize;
}

void GaussianBlurKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void GaussianBlurKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void GaussianBlurKernel::SetSigma(float t_x, float t_y, float t_z)
{
    this->m_Sigma = {t_x, t_y, t_z};
}

void GaussianBlurKernel::Execute()
{
    auto src = this->GetParameter<Object>("src");
    auto dst = this->GetParameter<Object>("dst");

    std::array<int,3> kernel_size = this->Sigma2KernelSize(this->m_Sigma);

    ExecuteSeparableKernel kernel(this->m_gpu);
    kernel.SetKernelName(this->m_KernelName);
    kernel.SetSources(this->m_Sources);
    kernel.SetInput(*src);
    kernel.SetOutput(*dst);
    kernel.SetSigma(this->m_Sigma[0], this->m_Sigma[1], this->m_Sigma[2]);
    kernel.SetKernelSize(kernel_size[0], kernel_size[1], kernel_size[2]);
    kernel.Execute();
}

} // namespace cle
