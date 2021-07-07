
#include "CLE.h"
#include "cleKernelList.h"


namespace cle
{

CLE::CLE(GPU& device)
{
    m_gpu = device;
}

GPU CLE::GetGPU()
{
    return this->m_gpu;
}

void CLE::AddImageAndScalar(Buffer& src, Buffer& dst, float scalar)
{
    AddImageAndScalarKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetScalar(scalar);
    kernel.Execute();
}

void CLE::AddImagesWeighted(Buffer& src1, Buffer& src2, Buffer& dst, float factor1, float factor2)
{
    AddImagesWeightedKernel kernel(this->m_gpu);
    kernel.SetInput1(src1);
    kernel.SetInput2(src2);
    kernel.SetOutput(dst);
    kernel.SetFactor1(factor1);
    kernel.SetFactor2(factor2);
    kernel.Execute();
}

void CLE::AddImages(Buffer& src1, Buffer& src2, Buffer& dst)
{
    this->AddImagesWeighted(src1, src2, dst, 1, 1);
}

void CLE::SubtractImages(Buffer& src1, Buffer& src2, Buffer& dst)
{
    this->AddImagesWeighted(src1, src2, dst, 1, -1);
}

void CLE::DilateSphere(Buffer& src, Buffer& dst)
{
    DilateSphereKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute();
}

void CLE::ErodeSphere(Buffer& src, Buffer& dst)
{
    ErodeSphereKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute();
}

void CLE::Equal(Buffer& src1, Buffer& src2, Buffer& dst)
{
    EqualKernel kernel(this->m_gpu);
    kernel.SetInput1(src1);
    kernel.SetInput2(src2);
    kernel.SetOutput(dst);
    kernel.Execute();
}

void CLE::EqualConstant(Buffer& src, Buffer& dst, float scalar)
{
    EqualConstantKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetScalar(scalar);
    kernel.Execute();
}

void CLE::GaussianBlur3D(Buffer& src, Buffer& dst, float sigmaX, float sigmaY, float sigmaZ)
{
    GaussianBlurKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetSigma(sigmaX, sigmaY, sigmaZ);
    kernel.Execute();
}

void CLE::GaussianBlur2D(Buffer& src, Buffer& dst, float sigmaX, float sigmaY)
{
    GaussianBlurKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetSigma(sigmaX, sigmaY, 0);
    kernel.Execute();
}

void CLE::Maximum3DBox(Buffer& src, Buffer& dst, float sigmaX, float sigmaY, float sigmaZ)
{
    MaximumBoxKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetRadius(sigmaX, sigmaY, sigmaZ);
    kernel.Execute();
}

void CLE::Maximum2DBox(Buffer& src, Buffer& dst, float sigmaX, float sigmaY)
{
    MaximumBoxKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetRadius(sigmaX, sigmaY, 0);
    kernel.Execute();
}

void CLE::Minimum3DBox(Buffer& src, Buffer& dst, float sigmaX, float sigmaY, float sigmaZ)
{
    MinimumBoxKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetRadius(sigmaX, sigmaY, sigmaZ);
    kernel.Execute();
}

void CLE::Minimum2DBox(Buffer& src, Buffer& dst, float sigmaX, float sigmaY)
{
    MinimumBoxKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetRadius(sigmaX, sigmaY, 0);
    kernel.Execute();
}

void CLE::Mean3DBox(Buffer& src, Buffer& dst, float radius_x, float radius_y, float radius_z)
{
    MeanBoxKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetRadius(radius_x, radius_y, radius_z);
    kernel.Execute();
}

void CLE::Mean2DBox(Buffer& src, Buffer& dst, float radius_x, float radius_y)
{
    MeanBoxKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetRadius(radius_x, radius_y, 0);
    kernel.Execute();
}

void CLE::Greater(Buffer& src1, Buffer& src2, Buffer& dst)
{
    GreaterKernel kernel(this->m_gpu);
    kernel.SetInput1(src1);
    kernel.SetInput2(src2);
    kernel.SetOutput(dst);
    kernel.Execute();
}

void CLE::GreaterOrEqual(Buffer& src1, Buffer& src2, Buffer& dst)
{
    GreaterOrEqualKernel kernel(this->m_gpu);
    kernel.SetInput1(src1);
    kernel.SetInput2(src2);
    kernel.SetOutput(dst);
    kernel.Execute();
}

void CLE::GreaterConstant(Buffer& src, Buffer& dst, float scalar)
{
    GreaterConstantKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetScalar(scalar);
    kernel.Execute();
}

void CLE::GreaterOrEqualConstant(Buffer& src, Buffer& dst, float scalar)
{
    GreaterOrEqualConstantKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetScalar(scalar);
    kernel.Execute();
}

void CLE::MaximumZProjection(Buffer& src, Buffer& dst)
{
    MaximumZProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}

void CLE::MaximumYProjection(Buffer& src, Buffer& dst)
{
    MaximumYProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}

void CLE::MaximumXProjection(Buffer& src, Buffer& dst)
{
    MaximumXProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}

void CLE::MaximumOfAllPixels(Buffer& src, Buffer& dst)
{
    MaximumOfAllPixelsKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}  

void CLE::MinimumZProjection(Buffer& src, Buffer& dst)
{
    MinimumZProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}

void CLE::MinimumYProjection(Buffer& src, Buffer& dst)
{
    MinimumYProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}

void CLE::MinimumXProjection(Buffer& src, Buffer& dst)
{
    MinimumXProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}

void CLE::MinimumOfAllPixels(Buffer& src, Buffer& dst)
{
    MinimumOfAllPixelsKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}  

void CLE::DifferenceOfGaussian3D(Buffer& src, Buffer& dst, float sigma1x, float sigma1y, float sigma1z, 
                                                         float sigma2x, float sigma2y, float sigma2z)
{
    DifferenceOfGaussianKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetSigma1(sigma1x, sigma1y, sigma1z);
    kernel.SetSigma2(sigma2x, sigma2y, sigma2z);
    kernel.Execute(); 
}

void CLE::DifferenceOfGaussian2D(Buffer& src, Buffer& dst, float sigma1x, float sigma1y, 
                                                         float sigma2x, float sigma2y)
{
    DifferenceOfGaussianKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetSigma1(sigma1x, sigma1y, 0);
    kernel.SetSigma2(sigma2x, sigma2y, 0);
    kernel.Execute(); 
}

void CLE::Mean2DSphere(Buffer& src, Buffer& dst, int radius_x, int radius_y)
{
    MeanSphereKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetRadiusX(radius_x);
    kernel.SetRadiusY(radius_y);
    kernel.Execute();
}

void CLE::Mean3DSphere(Buffer& src, Buffer& dst, int radius_x, int radius_y, int radius_z)
{
    MeanSphereKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetRadiusX(radius_x);
    kernel.SetRadiusY(radius_y);
    kernel.SetRadiusY(radius_z);
    kernel.Execute();
}

void CLE::NonzeroMinimumBox(Buffer& src, Buffer& flag, Buffer& dst)
{
    NonzeroMinimumBoxKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetOutputFlag(flag);
    kernel.Execute();  
}

void CLE::NotEqual(Buffer& src1, Buffer& src2, Buffer& dst)
{
    NotEqualKernel kernel(this->m_gpu);
    kernel.SetInput1(src1);
    kernel.SetInput2(src2);
    kernel.SetOutput(dst);
    kernel.Execute();
}

void CLE::NotEqualConstant(Buffer& src, Buffer& dst, float scalar)
{
    NotEqualConstantKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetScalar(scalar);
    kernel.Execute();
}

void CLE::Smaller(Buffer& src1, Buffer& src2, Buffer& dst)
{
    SmallerKernel kernel(this->m_gpu);
    kernel.SetInput1(src1);
    kernel.SetInput2(src2);
    kernel.SetOutput(dst);
    kernel.Execute();
}

void CLE::SmallerOrEqual(Buffer& src1, Buffer& src2, Buffer& dst)
{
    SmallerOrEqualKernel kernel(this->m_gpu);
    kernel.SetInput1(src1);
    kernel.SetInput2(src2);
    kernel.SetOutput(dst);
    kernel.Execute();
}

void CLE::SmallerConstant(Buffer& src, Buffer& dst, float cst)
{
    SmallerConstantKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetConstant(cst);
    kernel.Execute();  
}

void CLE::SmallerOrEqualConstant(Buffer& src, Buffer& dst, float cst)
{
    SmallerOrEqualConstantKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetConstant(cst);
    kernel.Execute();
}

void CLE::Absolute(Buffer& src, Buffer& dst)
{
    AbsoluteKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute();  
}

void CLE::Sobel(Buffer& src, Buffer& dst)
{
    SobelKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute();  
}

void CLE::Set(Buffer& src, float value)
{
    SetKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetValue(value);
    kernel.Execute();  
}

void CLE::SetNonzeroPixelsToPixelindex(Buffer& src, Buffer& dst)
{
    SetNonzeroPixelsToPixelindexKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetOffset(1);
    kernel.Execute();  
}

void CLE::DetectMaximaBox(Buffer& src, Buffer& dst)
{
    DetectMaximaKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute();  
}

void CLE::Copy(Buffer& src, Buffer& dst)
{
    CopyKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute();  
}

void CLE::SumZProjection(Buffer& src, Buffer& dst)
{
    SumZProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}

void CLE::SumYProjection(Buffer& src, Buffer& dst)
{
    SumYProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}

void CLE::SumXProjection(Buffer& src, Buffer& dst)
{
    SumXProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}

void CLE::SumOfAllPixels(Buffer& src, Buffer& dst)
{
    SumOfAllPixelsKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}  

void CLE::ConnectedComponentLabellingBox(Buffer& src, Buffer& dst)
{
    ConnectedComponentLabellingBoxKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}

void CLE::ReplaceIntensity(Buffer& src, Buffer& dst, float in, float out)
{
    ReplaceIntensityKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetInValue(in);
    kernel.SetOutValue(out);
    kernel.Execute(); 
}

void CLE::ReplaceIntensities(Buffer& src, Buffer& ref, Buffer& dst)
{
    ReplaceIntensitiesKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetMap(ref);
    kernel.Execute(); 
}

void CLE::SetColumn(Buffer& src, int column, float value)
{
    SetColumnKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetColumn(column);
    kernel.SetValue(value);
    kernel.Execute();   
}

void CLE::SumReductionX(Buffer& src, Buffer& dst, int blocksize)
{
    SumReductionXKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetBlocksize(blocksize);
    kernel.Execute();   
}

void CLE::BlockEnumerate(Buffer& src, Buffer& sum, Buffer& dst, int blocksize)
{
    BlockEnumerateKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetInputSums(sum);
    kernel.SetOutput(dst);
    kernel.SetBlocksize(blocksize);
    kernel.Execute();   
}

void CLE::FlagExistingLabels(Buffer& src, Buffer& dst)
{
    FlagExistingLabelsKernel kernel(this->m_gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}


}