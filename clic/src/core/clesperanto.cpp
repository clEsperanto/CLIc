#include "clesperanto.hpp"

#include "cleKernelList.hpp"
namespace cle
{

Clesperanto::Clesperanto()
  : device_(std::make_shared<Processor>())
{
  this->GetDevice()->SelectDevice();
}

auto
Clesperanto::GetDevice() const -> ProcessorPointer
{
  return this->device_;
}

auto
Clesperanto::AddImageAndScalar(const Image & t_src, const Image & t_dst, const float & t_scalar) -> void
{
  AddImageAndScalarKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.SetScalar(t_scalar);
  kernel.Execute();
}

auto
Clesperanto::AddImagesWeighted(const Image & t_src1,
                               const Image & t_src2,
                               const Image & t_dst,
                               const float & t_factor1,
                               const float & t_factor2) -> void
{
  AddImagesWeightedKernel kernel(this->GetDevice());
  kernel.SetInput1(t_src1);
  kernel.SetInput2(t_src2);
  kernel.SetOutput(t_dst);
  kernel.SetFactor1(t_factor1);
  kernel.SetFactor2(t_factor2);
  kernel.Execute();
}

auto
Clesperanto::AddImages(const Image & t_src1, const Image & t_src2, const Image & t_dst) -> void
{
  this->AddImagesWeighted(t_src1, t_src2, t_dst, 1, 1);
}

auto
Clesperanto::BinaryAnd(const Image & t_src1, const Image & t_src2, const Image & t_dst) -> void
{
  BinaryAndKernel kernel(this->GetDevice());
  kernel.SetInput1(t_src1);
  kernel.SetInput2(t_src2);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::BinaryOr(const Image & t_src1, const Image & t_src2, const Image & t_dst) -> void
{
  BinaryOrKernel kernel(this->GetDevice());
  kernel.SetInput1(t_src1);
  kernel.SetInput2(t_src2);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::BinaryNot(const Image & t_src, const Image & t_dst) -> void
{
  BinaryNotKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::BinarySubtract(const Image & t_src1, const Image & t_src2, const Image & t_dst) -> void
{
  BinarySubtractKernel kernel(this->GetDevice());
  kernel.SetInput1(t_src1);
  kernel.SetInput2(t_src2);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::BinaryXor(const Image & t_src1, const Image & t_src2, const Image & t_dst) -> void
{
  BinaryXorKernel kernel(this->GetDevice());
  kernel.SetInput1(t_src1);
  kernel.SetInput2(t_src2);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::SubtractImages(const Image & t_src1, const Image & t_src2, const Image & t_dst) -> void
{
  this->AddImagesWeighted(t_src1, t_src2, t_dst, 1, -1);
}

auto
Clesperanto::DilateSphere(const Image & t_src, const Image & t_dst) -> void
{
  DilateSphereKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::ErodeSphere(const Image & t_src, const Image & t_dst) -> void
{
  ErodeSphereKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::Equal(const Image & t_src1, const Image & t_src2, const Image & t_dst) -> void
{
  EqualKernel kernel(this->GetDevice());
  kernel.SetInput1(t_src1);
  kernel.SetInput2(t_src2);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::EqualConstant(const Image & t_src, const Image & t_dst, const float & t_scalar) -> void
{
  EqualConstantKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.SetScalar(t_scalar);
  kernel.Execute();
}

auto
Clesperanto::GaussianBlur(const Image & t_src,
                          const Image & t_dst,
                          const float & t_sigmaX,
                          const float & t_sigmaY,
                          const float & t_sigmaZ) -> void
{
  GaussianBlurKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.SetSigma(t_sigmaX, t_sigmaY, t_sigmaZ);
  kernel.Execute();
}

auto
Clesperanto::MaximumBox(const Image & t_src,
                        const Image & t_dst,
                        const int &   t_radius_x,
                        const int &   t_radius_y,
                        const int &   t_radius_z) -> void
{
  MaximumBoxKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.SetRadius(t_radius_x, t_radius_y, t_radius_z);
  kernel.Execute();
}

auto
Clesperanto::MinimumBox(const Image & t_src,
                        const Image & t_dst,
                        const int &   t_radius_x,
                        const int &   t_radius_y,
                        const int &   t_radius_z) -> void
{
  MinimumBoxKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.SetRadius(t_radius_x, t_radius_y, t_radius_z);
  kernel.Execute();
}

auto
Clesperanto::MeanBox(const Image & t_src,
                     const Image & t_dst,
                     const int &   t_radius_x,
                     const int &   t_radius_y,
                     const int &   t_radius_z) -> void
{
  MeanBoxKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.SetRadius(t_radius_x, t_radius_y, t_radius_z);
  kernel.Execute();
}

auto
Clesperanto::Greater(const Image & t_src1, const Image & t_src2, const Image & t_dst) -> void
{
  GreaterKernel kernel(this->GetDevice());
  kernel.SetInput1(t_src1);
  kernel.SetInput2(t_src2);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::GreaterOrEqual(const Image & t_src1, const Image & t_src2, const Image & t_dst) -> void
{
  GreaterOrEqualKernel kernel(this->GetDevice());
  kernel.SetInput1(t_src1);
  kernel.SetInput2(t_src2);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::GreaterConstant(const Image & t_src, const Image & t_dst, const float & t_scalar) -> void
{
  GreaterConstantKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.SetScalar(t_scalar);
  kernel.Execute();
}

auto
Clesperanto::GreaterOrEqualConstant(const Image & t_src, const Image & t_dst, const float & t_scalar) -> void
{
  GreaterOrEqualConstantKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.SetScalar(t_scalar);
  kernel.Execute();
}

auto
Clesperanto::Mask(const Image & t_src, const Image & t_mask, const Image & t_dst) -> void
{
  MaskKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetMask(t_mask);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::MaskedVoronoiLabeling(const Image & t_src, const Image & t_mask, const Image & t_dst) -> void
{
  MaskedVoronoiLabelingKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetMask(t_mask);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::MaximumZProjection(const Image & t_src, const Image & t_dst) -> void
{
  MaximumZProjectionKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::MaximumYProjection(const Image & t_src, const Image & t_dst) -> void
{
  MaximumYProjectionKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::MaximumXProjection(const Image & t_src, const Image & t_dst) -> void
{
  MaximumXProjectionKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::MaximumOfAllPixels(const Image & t_src, const Image & t_dst) -> void
{
  MaximumOfAllPixelsKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::MinimumZProjection(const Image & t_src, const Image & t_dst) -> void
{
  MinimumZProjectionKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::MinimumYProjection(const Image & t_src, const Image & t_dst) -> void
{
  MinimumYProjectionKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::MinimumXProjection(const Image & t_src, const Image & t_dst) -> void
{
  MinimumXProjectionKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::MinimumOfAllPixels(const Image & t_src, const Image & t_dst) -> void
{
  MinimumOfAllPixelsKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::DifferenceOfGaussian(const Image & t_src,
                                  const Image & t_dst,
                                  const float & t_sigma1_x,
                                  const float & t_sigma1_y,
                                  const float & t_sigma1_z,
                                  const float & t_sigma2_x,
                                  const float & t_sigma2_y,
                                  const float & t_sigma2_z) -> void
{
  DifferenceOfGaussianKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.SetSigma1(t_sigma1_x, t_sigma1_y, t_sigma1_z);
  kernel.SetSigma2(t_sigma2_x, t_sigma2_y, t_sigma2_z);
  kernel.Execute();
}

auto
Clesperanto::MeanSphere(const Image & t_src,
                        const Image & t_dst,
                        const int &   t_radius_x,
                        const int &   t_radius_y,
                        const int &   t_radius_z) -> void
{
  MeanSphereKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.SetRadius(t_radius_x, t_radius_y, t_radius_z);
  kernel.Execute();
}

auto
Clesperanto::NonzeroMinimumBox(const Image & t_src, const Image & t_flag, const Image & t_dst) -> void
{
  NonzeroMinimumBoxKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.SetOutputFlag(t_flag);
  kernel.Execute();
}

auto
Clesperanto::NotEqual(const Image & t_src1, const Image & t_src2, const Image & t_dst) -> void
{
  NotEqualKernel kernel(this->GetDevice());
  kernel.SetInput1(t_src1);
  kernel.SetInput2(t_src2);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::NotEqualConstant(const Image & t_src, const Image & t_dst, const float & t_scalar) -> void
{
  NotEqualConstantKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.SetScalar(t_scalar);
  kernel.Execute();
}

auto
Clesperanto::Smaller(const Image & t_src1, const Image & t_src2, const Image & t_dst) -> void
{
  SmallerKernel kernel(this->GetDevice());
  kernel.SetInput1(t_src1);
  kernel.SetInput2(t_src2);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::SmallerOrEqual(const Image & t_src1, const Image & t_src2, const Image & t_dst) -> void
{
  SmallerOrEqualKernel kernel(this->GetDevice());
  kernel.SetInput1(t_src1);
  kernel.SetInput2(t_src2);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::SmallerConstant(const Image & t_src, const Image & t_dst, const float & t_scalar) -> void
{
  SmallerConstantKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.SetConstant(t_scalar);
  kernel.Execute();
}

auto
Clesperanto::SmallerOrEqualConstant(const Image & t_src, const Image & t_dst, const float & t_scalar) -> void
{
  SmallerOrEqualConstantKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.SetConstant(t_scalar);
  kernel.Execute();
}

auto
Clesperanto::Absolute(const Image & t_src, const Image & t_dst) -> void
{
  AbsoluteKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::Sobel(const Image & t_src, const Image & t_dst) -> void
{
  SobelKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::Set(const Image & t_src, const float & t_scalar) -> void
{
  SetKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetValue(t_scalar);
  kernel.Execute();
}

auto
Clesperanto::SetNonzeroPixelsToPixelindex(const Image & t_src, const Image & t_dst) -> void
{
  SetNonzeroPixelsToPixelindexKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.SetOffset(1);
  kernel.Execute();
}

auto
Clesperanto::DetectMaximaBox(const Image & t_src, const Image & t_dst) -> void
{
  DetectMaximaKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  // kernel.SetRadius (t_radius_x, t_radius_y, t_radius_z);
  kernel.Execute();
}

auto
Clesperanto::Copy(const Image & t_src, const Image & t_dst) -> void
{
  CopyKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::SumZProjection(const Image & t_src, const Image & t_dst) -> void
{
  SumZProjectionKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::SumYProjection(const Image & t_src, const Image & t_dst) -> void
{
  SumYProjectionKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::SumXProjection(const Image & t_src, const Image & t_dst) -> void
{
  SumXProjectionKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::SumOfAllPixels(const Image & t_src, const Image & t_dst) -> void
{
  SumOfAllPixelsKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::ConnectedComponentLabelingBox(const Image & t_src, const Image & t_dst) -> void
{
  ConnectedComponentLabelingBoxKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::ReplaceIntensity(const Image & t_src, const Image & t_dst, const float & t_int_in, const float & t_int_out)
  -> void
{
  ReplaceIntensityKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.SetInValue(t_int_in);
  kernel.SetOutValue(t_int_out);
  kernel.Execute();
}

auto
Clesperanto::ReplaceIntensities(const Image & t_src, const Image & ref, const Image & t_dst) -> void
{
  ReplaceIntensitiesKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.SetMap(ref);
  kernel.Execute();
}

auto
Clesperanto::SetColumn(const Image & t_src, const int & t_column_idx, const float & t_scalar) -> void
{
  SetColumnKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetColumn(t_column_idx);
  kernel.SetValue(t_scalar);
  kernel.Execute();
}

auto
Clesperanto::SumReductionX(const Image & t_src, const Image & t_dst, const int & t_blocksize) -> void
{
  SumReductionXKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.SetBlocksize(t_blocksize);
  kernel.Execute();
}

auto
Clesperanto::BlockEnumerate(const Image & t_src, const Image & sum, const Image & t_dst, const int & t_blocksize)
  -> void
{
  BlockEnumerateKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetInputSums(sum);
  kernel.SetOutput(t_dst);
  kernel.SetBlocksize(t_blocksize);
  kernel.Execute();
}

auto
Clesperanto::FlagExistingLabels(const Image & t_src, const Image & t_dst) -> void
{
  FlagExistingLabelsKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::CloseIndexGapsInLabelMap(const Image & t_src, const Image & t_dst, const int & t_blocksize) -> void
{
  CloseIndexGapsInLabelMapKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.SetBlockSize(t_blocksize);
  kernel.Execute();
}

auto
Clesperanto::Histogram(const Image & t_src,
                       const Image & t_dst,
                       const int &   t_bins,
                       const float & t_min,
                       const float & t_max) -> void
{
  HistogramKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.SetSteps(1, 1, 1);
  kernel.SetNumBins(t_bins);
  kernel.SetMinimumIntensity(t_min);
  kernel.SetMaximumIntensity(t_max);
  kernel.Execute();
}

auto
Clesperanto::ThresholdOtsu(const Image & t_src, const Image & t_dst) -> void
{
  ThresholdOtsuKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::OnlyzeroOverwriteMaximumBox(const Image & t_src, const Image & t_dst1, const Image & t_dst2) -> void
{
  OnlyzeroOverwriteMaximumBoxKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput1(t_dst1);
  kernel.SetOutput2(t_dst2);
  kernel.Execute();
}

auto
Clesperanto::OnlyzeroOverwriteMaximumDiamond(const Image & t_src, const Image & t_dst1, const Image & t_dst2) -> void
{
  OnlyzeroOverwriteMaximumDiamondKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput1(t_dst1);
  kernel.SetOutput2(t_dst2);
  kernel.Execute();
}

auto
Clesperanto::VoronoiOtsuLabeling(const Image & t_src,
                                 const Image & t_dst,
                                 const float & t_sigma_spot,
                                 const float & t_sigma_outline) -> void
{
  VoronoiOtsuLabelingKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.SetSpotSigma(t_sigma_spot);
  kernel.SetOutlineSigma(t_sigma_outline);
  kernel.Execute();
}

auto
Clesperanto::ExtendLabelingViaVoronoi(const Image & t_src, const Image & t_dst) -> void
{
  ExtendLabelingViaVoronoiKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::TopHatBox(const Image & t_src,
                       const Image & t_dst,
                       const int &   t_radius_x,
                       const int &   t_radius_y,
                       const int &   t_radius_z) -> void
{
  TopHatBoxKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.SetRadius(t_radius_x, t_radius_y, t_radius_z);
  kernel.Execute();
}

auto
Clesperanto::MultiplyImages(const Image & t_src1, const Image & t_src2, const Image & t_dst) -> void
{
  MultiplyImagesKernel kernel(this->GetDevice());
  kernel.SetInput1(t_src1);
  kernel.SetInput2(t_src2);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::DivideImages(const Image & t_src1, const Image & t_src2, const Image & t_dst) -> void
{
  DivideImagesKernel kernel(this->GetDevice());
  kernel.SetInput1(t_src1);
  kernel.SetInput2(t_src2);
  kernel.SetOutput(t_dst);
  kernel.Execute();
}

auto
Clesperanto::SubtractImageFromScalar(const Image & t_src, const Image & t_dst, const float & t_scalar) -> void
{
  SubtractImageFromScalarKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.SetScalar(t_scalar);
  kernel.Execute();
}

auto
Clesperanto::DilateLabels(const Image & t_src, const Image & t_dst, const int & t_radius) -> void
{
  DilateLabelsKernel kernel(this->GetDevice());
  kernel.SetInput(t_src);
  kernel.SetOutput(t_dst);
  kernel.SetRadius(t_radius);
  kernel.Execute();
}

} // namespace cle