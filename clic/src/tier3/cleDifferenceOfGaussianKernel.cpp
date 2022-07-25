

#include "cleDifferenceOfGaussianKernel.hpp"
#include "cleAddImagesWeightedKernel.hpp"
#include "cleGaussianBlurKernel.hpp"

namespace cle
{

DifferenceOfGaussianKernel::DifferenceOfGaussianKernel (const ProcessorPointer &device) : Operation (device, 2)
{
}

void
DifferenceOfGaussianKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
DifferenceOfGaussianKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
DifferenceOfGaussianKernel::SetSigma1 (const float &sigma_x, const float &sigma_y, const float &sigma_z)
{
    this->sigma1_ = { sigma_x, sigma_y, sigma_z };
}

void
DifferenceOfGaussianKernel::SetSigma2 (const float &sigma_x, const float &sigma_y, const float &sigma_z)
{
    this->sigma2_ = { sigma_x, sigma_y, sigma_z };
}

void
DifferenceOfGaussianKernel::Execute ()
{
    // auto src = this->GetParameter<Object> ("src");
    // auto dst = this->GetParameter<Object> ("dst");

    // auto temp1 = this->m_gpu->Create<float> (src->Shape ());
    // auto temp2 = this->m_gpu->Create<float> (src->Shape ());

    // GaussianBlurKernel gaussian_1_kernel (this->m_gpu);
    // gaussian_1_kernel.SetInput (*src);
    // gaussian_1_kernel.SetOutput (temp1);
    // gaussian_1_kernel.SetSigma (this->sigma1_[0], this->sigma1_[1], this->sigma1_[2]);
    // gaussian_1_kernel.Execute ();

    // GaussianBlurKernel gaussian_2_kernel (this->m_gpu);
    // gaussian_2_kernel.SetInput (*src);
    // gaussian_2_kernel.SetOutput (temp2);
    // gaussian_2_kernel.SetSigma (this->sigma2_[0], this->sigma2_[1], this->sigma2_[2]);
    // gaussian_2_kernel.Execute ();

    // AddImagesWeightedKernel difference (this->m_gpu);
    // difference.SetInput1 (temp1);
    // difference.SetInput2 (temp2);
    // difference.SetOutput (*dst);
    // difference.SetFactor1 (1);
    // difference.SetFactor2 (-1);
    // difference.Execute ();
}

} // namespace cle
