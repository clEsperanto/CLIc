
#include "clesperanto.hpp"
#include "cleKernelList.hpp"


namespace cle
{

Clesperanto::Clesperanto()
{
    m_gpu = std::shared_ptr<GPU>(new GPU);
}

std::shared_ptr<GPU> Clesperanto::GetGPU()
{
    return this->m_gpu;
}

void Clesperanto::AddImageAndScalar(Buffer& src, Buffer& dst, float scalar)
{
    AddImageAndScalarKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetScalar(scalar);
    kernel.Execute();
}

void Clesperanto::AddImagesWeighted(Buffer& src1, Buffer& src2, Buffer& dst, float factor1, float factor2)
{
    AddImagesWeightedKernel kernel(this->m_gpu);
    kernel.SetInput1(src1);
    kernel.SetInput2(src2);
    kernel.SetOutput(dst);
    kernel.SetFactor1(factor1);
    kernel.SetFactor2(factor2);
    kernel.Execute();
}

void Clesperanto::AddImages(Buffer& src1, Buffer& src2, Buffer& dst)
{
    this->AddImagesWeighted(src1, src2, dst, 1, 1);
}

void Clesperanto::SubtractImages(Buffer& src1, Buffer& src2, Buffer& dst)
{
    this->AddImagesWeighted(src1, src2, dst, 1, -1);
}

void Clesperanto::DilateSphere(Buffer& src, Buffer& dst)
{
    DilateSphereKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute();
}

void Clesperanto::ErodeSphere(Buffer& src, Buffer& dst)
{
    ErodeSphereKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute();
}

void Clesperanto::Equal(Buffer& src1, Buffer& src2, Buffer& dst)
{
    EqualKernel kernel(this->m_gpu);
    kernel.SetInput1(src1);
    kernel.SetInput2(src2);
    kernel.SetOutput(dst);
    kernel.Execute();
}

void Clesperanto::EqualConstant(Buffer& src, Buffer& dst, float scalar)
{
    EqualConstantKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetScalar(scalar);
    kernel.Execute();
}

void Clesperanto::GaussianBlur(Buffer& src, Buffer& dst, float sigmaX, float sigmaY, float sigmaZ)
{
    GaussianBlurKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetSigma(sigmaX, sigmaY, sigmaZ);
    kernel.Execute();
}

void Clesperanto::MaximumBox(Buffer& src, Buffer& dst, float sigmaX, float sigmaY, float sigmaZ)
{
    MaximumBoxKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetRadius(sigmaX, sigmaY, sigmaZ);
    kernel.Execute();
}

void Clesperanto::MinimumBox(Buffer& src, Buffer& dst, float sigmaX, float sigmaY, float sigmaZ)
{
    MinimumBoxKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetRadius(sigmaX, sigmaY, sigmaZ);
    kernel.Execute();
}

void Clesperanto::MeanBox(Buffer& src, Buffer& dst, float radius_x, float radius_y, float radius_z)
{
    MeanBoxKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetRadius(radius_x, radius_y, radius_z);
    kernel.Execute();
}

void Clesperanto::Greater(Buffer& src1, Buffer& src2, Buffer& dst)
{
    GreaterKernel kernel(this->m_gpu);
    kernel.SetInput1(src1);
    kernel.SetInput2(src2);
    kernel.SetOutput(dst);
    kernel.Execute();
}

void Clesperanto::GreaterOrEqual(Buffer& src1, Buffer& src2, Buffer& dst)
{
    GreaterOrEqualKernel kernel(this->m_gpu);
    kernel.SetInput1(src1);
    kernel.SetInput2(src2);
    kernel.SetOutput(dst);
    kernel.Execute();
}

void Clesperanto::GreaterConstant(Buffer& src, Buffer& dst, float scalar)
{
    GreaterConstantKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetScalar(scalar);
    kernel.Execute();
}

void Clesperanto::GreaterOrEqualConstant(Buffer& src, Buffer& dst, float scalar)
{
    GreaterOrEqualConstantKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetScalar(scalar);
    kernel.Execute();
}

void Clesperanto::MaximumZProjection(Buffer& src, Buffer& dst)
{
    MaximumZProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}

void Clesperanto::MaximumYProjection(Buffer& src, Buffer& dst)
{
    MaximumYProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}

void Clesperanto::MaximumXProjection(Buffer& src, Buffer& dst)
{
    MaximumXProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}

void Clesperanto::MaximumOfAllPixels(Buffer& src, Buffer& dst)
{
    MaximumOfAllPixelsKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}  

void Clesperanto::MinimumZProjection(Buffer& src, Buffer& dst)
{
    MinimumZProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}

void Clesperanto::MinimumYProjection(Buffer& src, Buffer& dst)
{
    MinimumYProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}

void Clesperanto::MinimumXProjection(Buffer& src, Buffer& dst)
{
    MinimumXProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}

void Clesperanto::MinimumOfAllPixels(Buffer& src, Buffer& dst)
{
    MinimumOfAllPixelsKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}  

void Clesperanto::DifferenceOfGaussian(Buffer& src, Buffer& dst, float sigma1x, float sigma1y, float sigma1z, 
                                                         float sigma2x, float sigma2y, float sigma2z)
{
    DifferenceOfGaussianKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetSigma1(sigma1x, sigma1y, sigma1z);
    kernel.SetSigma2(sigma2x, sigma2y, sigma2z);
    kernel.Execute(); 
}

void Clesperanto::MeanSphere(Buffer& src, Buffer& dst, int radius_x, int radius_y, int radius_z)
{
    MeanSphereKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetRadius(radius_x, radius_y, radius_z);
    kernel.Execute();
}

void Clesperanto::NonzeroMinimumBox(Buffer& src, Buffer& flag, Buffer& dst)
{
    NonzeroMinimumBoxKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetOutputFlag(flag);
    kernel.Execute();  
}

void Clesperanto::NotEqual(Buffer& src1, Buffer& src2, Buffer& dst)
{
    NotEqualKernel kernel(this->m_gpu);
    kernel.SetInput1(src1);
    kernel.SetInput2(src2);
    kernel.SetOutput(dst);
    kernel.Execute();
}

void Clesperanto::NotEqualConstant(Buffer& src, Buffer& dst, float scalar)
{
    NotEqualConstantKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetScalar(scalar);
    kernel.Execute();
}

void Clesperanto::Smaller(Buffer& src1, Buffer& src2, Buffer& dst)
{
    SmallerKernel kernel(this->m_gpu);
    kernel.SetInput1(src1);
    kernel.SetInput2(src2);
    kernel.SetOutput(dst);
    kernel.Execute();
}

void Clesperanto::SmallerOrEqual(Buffer& src1, Buffer& src2, Buffer& dst)
{
    SmallerOrEqualKernel kernel(this->m_gpu);
    kernel.SetInput1(src1);
    kernel.SetInput2(src2);
    kernel.SetOutput(dst);
    kernel.Execute();
}

void Clesperanto::SmallerConstant(Buffer& src, Buffer& dst, float cst)
{
    SmallerConstantKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetConstant(cst);
    kernel.Execute();  
}

void Clesperanto::SmallerOrEqualConstant(Buffer& src, Buffer& dst, float cst)
{
    SmallerOrEqualConstantKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetConstant(cst);
    kernel.Execute();
}

void Clesperanto::Absolute(Buffer& src, Buffer& dst)
{
    AbsoluteKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute();  
}

void Clesperanto::Sobel(Buffer& src, Buffer& dst)
{
    SobelKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute();  
}

void Clesperanto::Set(Buffer& src, float value)
{
    SetKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetValue(value);
    kernel.Execute();  
}

void Clesperanto::SetNonzeroPixelsToPixelindex(Buffer& src, Buffer& dst)
{
    SetNonzeroPixelsToPixelindexKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetOffset(1);
    kernel.Execute();  
}

void Clesperanto::DetectMaximaBox(Buffer& src, Buffer& dst)
{
    DetectMaximaKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute();  
}

void Clesperanto::Copy(Buffer& src, Buffer& dst)
{
    CopyKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute();  
}

void Clesperanto::SumZProjection(Buffer& src, Buffer& dst)
{
    SumZProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}

void Clesperanto::SumYProjection(Buffer& src, Buffer& dst)
{
    SumYProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}

void Clesperanto::SumXProjection(Buffer& src, Buffer& dst)
{
    SumXProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}

void Clesperanto::SumOfAllPixels(Buffer& src, Buffer& dst)
{
    SumOfAllPixelsKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}  

void Clesperanto::ConnectedComponentLabellingBox(Buffer& src, Buffer& dst)
{
    ConnectedComponentLabellingBoxKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}

void Clesperanto::ReplaceIntensity(Buffer& src, Buffer& dst, float in, float out)
{
    ReplaceIntensityKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetInValue(in);
    kernel.SetOutValue(out);
    kernel.Execute(); 
}

void Clesperanto::ReplaceIntensities(Buffer& src, Buffer& ref, Buffer& dst)
{
    ReplaceIntensitiesKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetMap(ref);
    kernel.Execute(); 
}

void Clesperanto::SetColumn(Buffer& src, int column, float value)
{
    SetColumnKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetColumn(column);
    kernel.SetValue(value);
    kernel.Execute();   
}

void Clesperanto::SumReductionX(Buffer& src, Buffer& dst, int blocksize)
{
    SumReductionXKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetBlocksize(blocksize);
    kernel.Execute();   
}

void Clesperanto::BlockEnumerate(Buffer& src, Buffer& sum, Buffer& dst, int blocksize)
{
    BlockEnumerateKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetInputSums(sum);
    kernel.SetOutput(dst);
    kernel.SetBlocksize(blocksize);
    kernel.Execute();   
}

void Clesperanto::FlagExistingLabels(Buffer& src, Buffer& dst)
{
    FlagExistingLabelsKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}


}