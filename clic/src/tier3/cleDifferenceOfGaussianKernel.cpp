

#include "cleDifferenceOfGaussianKernel.hpp"
#include "cleGaussianBlurKernel.hpp"
#include "cleAddImagesWeightedKernel.hpp"

namespace cle
{

void DifferenceOfGaussianKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void DifferenceOfGaussianKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void DifferenceOfGaussianKernel::SetSigma1(float x, float y, float z)
{
    this->sigma1[0] = x;
    this->sigma1[1] = y;
    this->sigma1[2] = z;
}

void DifferenceOfGaussianKernel::SetSigma2(float x, float y, float z)
{
    this->sigma2[0] = x;
    this->sigma2[1] = y;
    this->sigma2[2] = z;
}

void DifferenceOfGaussianKernel::Execute()
{
    std::shared_ptr<Buffer> src = std::dynamic_pointer_cast<Buffer>(m_ParameterList.at("src"));
    std::shared_ptr<Buffer> dst = std::dynamic_pointer_cast<Buffer>(m_ParameterList.at("dst"));
    
    size_t size = src->GetDimensions()[0] * src->GetDimensions()[1] * src->GetDimensions()[2];
    cl::Buffer tmp1_obj = CreateBuffer<float>(src->GetSize(), this->m_gpu);
    Buffer temp1 (tmp1_obj, src->GetDimensions(), LightObject::Float);
    cl::Buffer tmp2_obj = CreateBuffer<float>(src->GetSize(), this->m_gpu);
    Buffer temp2 (tmp2_obj, src->GetDimensions(), LightObject::Float);

    GaussianBlurKernel gaussian1(this->m_gpu);
    gaussian1.SetInput(*src);
    gaussian1.SetOutput(temp1);
    gaussian1.SetSigma(this->sigma1[0], this->sigma1[1], this->sigma1[2]);
    gaussian1.Execute();

    GaussianBlurKernel gaussian2(this->m_gpu);
    gaussian2.SetInput(*src);
    gaussian2.SetOutput(temp2);
    gaussian2.SetSigma(this->sigma2[0], this->sigma2[1], this->sigma2[2]);
    gaussian2.Execute();

    AddImagesWeightedKernel difference(this->m_gpu);
    difference.SetInput1(temp1);
    difference.SetInput2(temp2);
    difference.SetOutput(*dst);
    difference.SetFactor1(1);
    difference.SetFactor2(-1);
    difference.Execute();
}

} // namespace cle
