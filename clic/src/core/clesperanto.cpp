
#include "clesperanto.hpp"
#include "cleKernelList.hpp"

#include <iostream>
namespace cle
{

Clesperanto::Clesperanto() : m_gpu(std::make_shared<cle::GPU>())
{}

std::shared_ptr<GPU> Clesperanto::Ressources()
{
    return this->m_gpu;
}

void Clesperanto::AddImageAndScalar(Object& t_src, Object& t_dst, float t_scalar)
{
    AddImageAndScalarKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.SetScalar(t_scalar);
    kernel.Execute();
}

void Clesperanto::AddImagesWeighted(Object& t_src1, Object& t_src2, Object& t_dst, float t_factor1, float t_factor2)
{
    AddImagesWeightedKernel kernel(this->m_gpu);
    kernel.SetInput1(t_src1);
    kernel.SetInput2(t_src2);
    kernel.SetOutput(t_dst);
    kernel.SetFactor1(t_factor1);
    kernel.SetFactor2(t_factor2);
    kernel.Execute();
}

void Clesperanto::AddImages(Object& t_src1, Object& t_src2, Object& t_dst)
{
    this->AddImagesWeighted(t_src1, t_src2, t_dst, 1, 1);
}

void Clesperanto::BinaryAnd(Object& t_src1, Object& t_src2, Object& t_dst)
{
    BinaryAndKernel kernel(this->m_gpu);
    kernel.SetInput1(t_src1);
    kernel.SetInput2(t_src2);
    kernel.SetOutput(t_dst);
    kernel.Execute();
}

void Clesperanto::BinaryOr(Object& t_src1, Object& t_src2, Object& t_dst)
{
    BinaryOrKernel kernel(this->m_gpu);
    kernel.SetInput1(t_src1);
    kernel.SetInput2(t_src2);
    kernel.SetOutput(t_dst);
    kernel.Execute();
}

void Clesperanto::BinaryNot(Object& t_src, Object& t_dst)
{
    BinaryNotKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.Execute();
}

void Clesperanto::BinarySubtract(Object& t_src1, Object& t_src2, Object& t_dst)
{
    BinarySubtractKernel kernel(this->m_gpu);
    kernel.SetInput1(t_src1);
    kernel.SetInput2(t_src2);
    kernel.SetOutput(t_dst);
    kernel.Execute();
}

void Clesperanto::BinaryXor(Object& t_src1, Object& t_src2, Object& t_dst)
{
    BinaryXorKernel kernel(this->m_gpu);
    kernel.SetInput1(t_src1);
    kernel.SetInput2(t_src2);
    kernel.SetOutput(t_dst);
    kernel.Execute();
}

void Clesperanto::SubtractImages(Object& t_src1, Object& t_src2, Object& t_dst)
{
    this->AddImagesWeighted(t_src1, t_src2, t_dst, 1, -1);
}

void Clesperanto::DilateSphere(Object& t_src, Object& t_dst)
{
    DilateSphereKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.Execute();
}

void Clesperanto::ErodeSphere(Object& t_src, Object& t_dst)
{
    ErodeSphereKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.Execute();
}

void Clesperanto::Equal(Object& t_src1, Object& t_src2, Object& t_dst)
{
    EqualKernel kernel(this->m_gpu);
    kernel.SetInput1(t_src1);
    kernel.SetInput2(t_src2);
    kernel.SetOutput(t_dst);
    kernel.Execute();
}

void Clesperanto::EqualConstant(Object& t_src, Object& t_dst, float t_scalar)
{
    EqualConstantKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.SetScalar(t_scalar);
    kernel.Execute();
}

void Clesperanto::GaussianBlur(Object& t_src, Object& t_dst, float t_sigmaX, float t_sigmaY, float t_sigmaZ)
{
    GaussianBlurKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.SetSigma(t_sigmaX, t_sigmaY, t_sigmaZ);
    kernel.Execute();
}

void Clesperanto::MaximumBox(Object& t_src, Object& t_dst, float t_radius_x, float t_radius_y, float t_radius_z)
{
    MaximumBoxKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.SetRadius(t_radius_x, t_radius_y, t_radius_z);
    kernel.Execute();
}

void Clesperanto::MinimumBox(Object& t_src, Object& t_dst, float t_radius_x, float t_radius_y, float t_radius_z)
{
    MinimumBoxKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.SetRadius(t_radius_x, t_radius_y, t_radius_z);
    kernel.Execute();
}

void Clesperanto::MeanBox(Object& t_src, Object& t_dst, float t_radius_x, float t_radius_y, float t_radius_z)
{
    MeanBoxKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.SetRadius(t_radius_x, t_radius_y, t_radius_z);
    kernel.Execute();
}

void Clesperanto::Greater(Object& t_src1, Object& t_src2, Object& t_dst)
{
    GreaterKernel kernel(this->m_gpu);
    kernel.SetInput1(t_src1);
    kernel.SetInput2(t_src2);
    kernel.SetOutput(t_dst);
    kernel.Execute();
}

void Clesperanto::GreaterOrEqual(Object& t_src1, Object& t_src2, Object& t_dst)
{
    GreaterOrEqualKernel kernel(this->m_gpu);
    kernel.SetInput1(t_src1);
    kernel.SetInput2(t_src2);
    kernel.SetOutput(t_dst);
    kernel.Execute();
}

void Clesperanto::GreaterConstant(Object& t_src, Object& t_dst, float t_scalar)
{
    GreaterConstantKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.SetScalar(t_scalar);
    kernel.Execute();
}

void Clesperanto::GreaterOrEqualConstant(Object& t_src, Object& t_dst, float t_scalar)
{
    GreaterOrEqualConstantKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.SetScalar(t_scalar);
    kernel.Execute();
}

void Clesperanto::Mask(Object& t_src, Object& t_mask, Object& t_dst)
{
    MaskKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetMask(t_mask);
    kernel.SetOutput(t_dst);
    kernel.Execute();  
}

void Clesperanto::MaskedVoronoiLabeling(Object& t_src, Object& t_mask, Object& t_dst)
{
    MaskedVoronoiLabelingKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetMask(t_mask);
    kernel.SetOutput(t_dst);
    kernel.Execute();  
}

void Clesperanto::MaximumZProjection(Object& t_src, Object& t_dst)
{
    MaximumZProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.Execute(); 
}

void Clesperanto::MaximumYProjection(Object& t_src, Object& t_dst)
{
    MaximumYProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.Execute(); 
}

void Clesperanto::MaximumXProjection(Object& t_src, Object& t_dst)
{
    MaximumXProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.Execute(); 
}

void Clesperanto::MaximumOfAllPixels(Object& t_src, Object& t_dst)
{
    MaximumOfAllPixelsKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.Execute(); 
}  

void Clesperanto::MinimumZProjection(Object& t_src, Object& t_dst)
{
    MinimumZProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.Execute(); 
}

void Clesperanto::MinimumYProjection(Object& t_src, Object& t_dst)
{
    MinimumYProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.Execute(); 
}

void Clesperanto::MinimumXProjection(Object& t_src, Object& t_dst)
{
    MinimumXProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.Execute(); 
}

void Clesperanto::MinimumOfAllPixels(Object& t_src, Object& t_dst)
{
    MinimumOfAllPixelsKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.Execute(); 
}  

void Clesperanto::DifferenceOfGaussian(Object& t_src, Object& t_dst, float t_sigma1_x, float t_sigma1_y, float t_sigma1_z, 
                                                         float t_sigma2_x, float t_sigma2_y, float t_sigma2_z)
{
    DifferenceOfGaussianKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.SetSigma1(t_sigma1_x, t_sigma1_y, t_sigma1_z);
    kernel.SetSigma2(t_sigma2_x, t_sigma2_y, t_sigma2_z);
    kernel.Execute(); 
}

void Clesperanto::MeanSphere(Object& t_src, Object& t_dst, int t_radius_x, int t_radius_y, int t_radius_z)
{
    MeanSphereKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.SetRadius(t_radius_x, t_radius_y, t_radius_z);
    kernel.Execute();
}

void Clesperanto::NonzeroMinimumBox(Object& t_src, Object& t_flag, Object& t_dst)
{
    NonzeroMinimumBoxKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.SetOutputFlag(t_flag);
    kernel.Execute();  
}

void Clesperanto::NotEqual(Object& t_src1, Object& t_src2, Object& t_dst)
{
    NotEqualKernel kernel(this->m_gpu);
    kernel.SetInput1(t_src1);
    kernel.SetInput2(t_src2);
    kernel.SetOutput(t_dst);
    kernel.Execute();
}

void Clesperanto::NotEqualConstant(Object& t_src, Object& t_dst, float t_scalar)
{
    NotEqualConstantKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.SetScalar(t_scalar);
    kernel.Execute();
}

void Clesperanto::Smaller(Object& t_src1, Object& t_src2, Object& t_dst)
{
    SmallerKernel kernel(this->m_gpu);
    kernel.SetInput1(t_src1);
    kernel.SetInput2(t_src2);
    kernel.SetOutput(t_dst);
    kernel.Execute();
}

void Clesperanto::SmallerOrEqual(Object& t_src1, Object& t_src2, Object& t_dst)
{
    SmallerOrEqualKernel kernel(this->m_gpu);
    kernel.SetInput1(t_src1);
    kernel.SetInput2(t_src2);
    kernel.SetOutput(t_dst);
    kernel.Execute();
}

void Clesperanto::SmallerConstant(Object& t_src, Object& t_dst, float t_scalar)
{
    SmallerConstantKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.SetConstant(t_scalar);
    kernel.Execute();  
}

void Clesperanto::SmallerOrEqualConstant(Object& t_src, Object& t_dst, float t_scalar)
{
    SmallerOrEqualConstantKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.SetConstant(t_scalar);
    kernel.Execute();
}

void Clesperanto::Absolute(Object& t_src, Object& t_dst)
{
    AbsoluteKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.Execute();  
}

void Clesperanto::Sobel(Object& t_src, Object& t_dst)
{
    SobelKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.Execute();  
}

void Clesperanto::Set(Object& t_src, float t_scalar)
{
    SetKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetValue(t_scalar);
    kernel.Execute();  
}

void Clesperanto::SetNonzeroPixelsToPixelindex(Object& t_src, Object& t_dst)
{
    SetNonzeroPixelsToPixelindexKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.SetOffset(1);
    kernel.Execute();  
}

void Clesperanto::DetectMaximaBox(Object& t_src, Object& t_dst, int t_radius_x, int t_radius_y, int t_radius_z)
{
    DetectMaximaKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.SetRadius(t_radius_x, t_radius_y, t_radius_z);
    kernel.Execute();  
}

void Clesperanto::Copy(Object& t_src, Object& t_dst)
{
    CopyKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.Execute();  
}

void Clesperanto::SumZProjection(Object& t_src, Object& t_dst)
{
    SumZProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.Execute(); 
}

void Clesperanto::SumYProjection(Object& t_src, Object& t_dst)
{
    SumYProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.Execute(); 
}

void Clesperanto::SumXProjection(Object& t_src, Object& t_dst)
{
    SumXProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.Execute(); 
}

void Clesperanto::SumOfAllPixels(Object& t_src, Object& t_dst)
{
    SumOfAllPixelsKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.Execute(); 
}  

void Clesperanto::ConnectedComponentsLabelingBox(Object& t_src, Object& t_dst)
{
    ConnectedComponentsLabelingBoxKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.Execute(); 
}

void Clesperanto::ReplaceIntensity(Object& t_src, Object& t_dst, float t_int_in, float t_int_out)
{
    ReplaceIntensityKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.SetInValue(t_int_in);
    kernel.SetOutValue(t_int_out);
    kernel.Execute(); 
}

void Clesperanto::ReplaceIntensities(Object& t_src, Object& ref, Object& t_dst)
{
    ReplaceIntensitiesKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.SetMap(ref);
    kernel.Execute(); 
}

void Clesperanto::SetColumn(Object& t_src, int t_column_idx, float t_scalar)
{
    SetColumnKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetColumn(t_column_idx);
    kernel.SetValue(t_scalar);
    kernel.Execute();   
}

void Clesperanto::SumReductionX(Object& t_src, Object& t_dst, int t_blocksize)
{
    SumReductionXKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.SetBlocksize(t_blocksize);
    kernel.Execute();   
}

void Clesperanto::BlockEnumerate(Object& t_src, Object& sum, Object& t_dst, int t_blocksize)
{
    BlockEnumerateKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetInputSums(sum);
    kernel.SetOutput(t_dst);
    kernel.SetBlocksize(t_blocksize);
    kernel.Execute();   
}

void Clesperanto::FlagExistingLabels(Object& t_src, Object& t_dst)
{
    FlagExistingLabelsKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.Execute(); 
}

void Clesperanto::CloseIndexGapsInLabelMap(Object& t_src, Object& t_dst, int t_blocksize)
{
    CloseIndexGapsInLabelMapKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.SetBlockSize(t_blocksize);
    kernel.Execute();
}

void Clesperanto::Histogram(Object& t_src, Object& t_dst, int t_bins, float t_min, float t_max)
{
    HistogramKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.SetSteps(1, 1, 1);
    kernel.SetNumBins(t_bins);
    kernel.SetMinimumIntensity(t_min);
    kernel.SetMaximumIntensity(t_max);
    kernel.Execute();
}

void Clesperanto::ThresholdOtsu(Object& t_src, Object& t_dst)
{
    ThresholdOtsuKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.Execute(); 
}

void Clesperanto::OnlyzeroOverwriteMaximumBox(Object& t_src, Object& t_dst1, Object& t_dst2)
{
    OnlyzeroOverwriteMaximumBoxKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput1(t_dst1);
    kernel.SetOutput2(t_dst2);
    kernel.Execute();
}   

void Clesperanto::OnlyzeroOverwriteMaximumDiamond(Object& t_src, Object& t_dst1, Object& t_dst2)
{
    OnlyzeroOverwriteMaximumDiamondKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput1(t_dst1);
    kernel.SetOutput2(t_dst2);
    kernel.Execute();
}

void Clesperanto::VoronoiOtsuLabeling(Object& t_src, Object& t_dst, float t_sigma_spot, float t_sigma_outline)
{
    VoronoiOtsuLabelingKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.SetSpotSigma(t_sigma_spot);
    kernel.SetOutlineSigma(t_sigma_outline);
    kernel.Execute();
}

void Clesperanto::ExtendLabelingViaVoronoi(Object& t_src, Object& t_dst)
{
    ExtendLabelingViaVoronoiKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.Execute();
}

void Clesperanto::TopHatBox(Object& t_src, Object& t_dst, float t_radius_x, float t_radius_y, float t_radius_z)
{
    TopHatBoxKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.SetRadius(t_radius_x, t_radius_y, t_radius_z);
    kernel.Execute();
}

void Clesperanto::MultiplyImages(Object& t_src1, Object& t_src2, Object& t_dst)
{
    MultiplyImagesKernel kernel(this->m_gpu);
    kernel.SetInput1(t_src1);
    kernel.SetInput2(t_src2);
    kernel.SetOutput(t_dst);
    kernel.Execute();
}

void Clesperanto::DivideImages(Object& t_src1, Object& t_src2, Object& t_dst)
{
    DivideImagesKernel kernel(this->m_gpu);
    kernel.SetInput1(t_src1);
    kernel.SetInput2(t_src2);
    kernel.SetOutput(t_dst);
    kernel.Execute();
}

void Clesperanto::SubtractImageFromScalar(Object& t_src, Object& t_dst, float t_scalar)
{
    SubtractImageFromScalarKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.SetScalar(t_scalar);
    kernel.Execute();
}

void Clesperanto::DilateLabels(Object& t_src, Object& t_dst, int t_radius)
{
    DilateLabelsKernel kernel(this->m_gpu);
    kernel.SetInput(t_src);
    kernel.SetOutput(t_dst);
    kernel.SetRadius(t_radius);
    kernel.Execute();
}


}