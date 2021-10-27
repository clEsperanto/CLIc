

#include "cleDifferenceOfGaussianKernel.hpp"
#include "cleGaussianBlurKernel.hpp"
#include "cleAddImagesWeightedKernel.hpp"

namespace cle
{

DifferenceOfGaussianKernel::DifferenceOfGaussianKernel(std::shared_ptr<GPU> t_gpu) :
    Kernel( t_gpu,
            "difference_of_gaussians",
            {"src", "dst"}
    )
{}

void DifferenceOfGaussianKernel::SetInput(Buffer& t_x)
{
    this->AddObject(t_x, "src");
}

void DifferenceOfGaussianKernel::SetOutput(Buffer& t_x)
{
    this->AddObject(t_x, "dst");
}

void DifferenceOfGaussianKernel::SetSigma1(float t_x, float t_y, float t_z)
{
    this->m_Sigma1[0] = t_x;
    this->m_Sigma1[1] = t_y;
    this->m_Sigma1[2] = t_z;
}

void DifferenceOfGaussianKernel::SetSigma2(float t_x, float t_y, float t_z)
{
    this->m_Sigma2[0] = t_x;
    this->m_Sigma2[1] = t_y;
    this->m_Sigma2[2] = t_z;
}

void DifferenceOfGaussianKernel::Execute()
{
    std::shared_ptr<Buffer> src = std::dynamic_pointer_cast<Buffer>(this->m_Parameters.at("src"));
    std::shared_ptr<Buffer> dst = std::dynamic_pointer_cast<Buffer>(this->m_Parameters.at("dst"));

    cle::Buffer temp1 = this->m_gpu->CreateBuffer<float>(src->Shape());
    cle::Buffer temp2 = this->m_gpu->CreateBuffer<float>(src->Shape());

    GaussianBlurKernel gaussian_1_kernel(this->m_gpu);
    gaussian_1_kernel.SetInput(*src);
    gaussian_1_kernel.SetOutput(temp1);
    gaussian_1_kernel.SetSigma(this->m_Sigma1[0], this->m_Sigma1[1], this->m_Sigma1[2]);
    gaussian_1_kernel.Execute();

    GaussianBlurKernel gaussian_2_kernel(this->m_gpu);
    gaussian_2_kernel.SetInput(*src);
    gaussian_2_kernel.SetOutput(temp2);
    gaussian_2_kernel.SetSigma(this->m_Sigma2[0], this->m_Sigma2[1], this->m_Sigma2[2]);
    gaussian_2_kernel.Execute();

    AddImagesWeightedKernel difference(this->m_gpu);
    difference.SetInput1(temp1);
    difference.SetInput2(temp2);
    difference.SetOutput(*dst);
    difference.SetFactor1(1);
    difference.SetFactor2(-1);
    difference.Execute();
}

} // namespace cle
