#include "clesperanto.hpp"

#include "cleKernelList.hpp"
namespace cle
{

Clesperanto::Clesperanto()
  : device_(std::make_shared<Processor>())
{
  if (Processor::ListAvailableDevices().empty())
  {
    std::cerr << "Error in initialising clEsperanto. No available device found." << std::endl;
    throw std::runtime_error("Error in initialising clEsperanto. No available device found.");
  }
  this->GetDevice()->SelectDevice();
}

auto
Clesperanto::GetDevice() const -> ProcessorPointer
{
  return this->device_;
}

auto
Clesperanto::WaitForKernelToFinish(const bool & flag) -> void
{
  this->GetDevice()->WaitForKernelToFinish(flag);
}

auto
Clesperanto::SelectDevice(const std::string & name) -> void
{
  this->GetDevice()->SelectDevice(name);
}

auto
Clesperanto::Info() -> std::string
{
  return this->GetDevice()->GetDeviceInfo();
}

auto
Clesperanto::AddImageAndScalar(const Image & source, const Image & destination, const float & scalar) -> void
{
  AddImageAndScalarKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.SetScalar(scalar);
  kernel.Execute();
}

auto
Clesperanto::AddImagesWeighted(const Image & source1,
                               const Image & source2,
                               const Image & destination,
                               const float & weight1,
                               const float & weight2) -> void
{
  AddImagesWeightedKernel kernel(this->GetDevice());
  kernel.SetInput1(source1);
  kernel.SetInput2(source2);
  kernel.SetOutput(destination);
  kernel.SetFactor1(weight1);
  kernel.SetFactor2(weight2);
  kernel.Execute();
}

auto
Clesperanto::AddImages(const Image & source1, const Image & source2, const Image & destination) -> void
{
  this->AddImagesWeighted(source1, source2, destination, 1, 1);
}

auto
Clesperanto::BinaryAnd(const Image & source1, const Image & source2, const Image & destination) -> void
{
  BinaryAndKernel kernel(this->GetDevice());
  kernel.SetInput1(source1);
  kernel.SetInput2(source2);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::BinaryOr(const Image & source1, const Image & source2, const Image & destination) -> void
{
  BinaryOrKernel kernel(this->GetDevice());
  kernel.SetInput1(source1);
  kernel.SetInput2(source2);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::BinaryNot(const Image & source, const Image & destination) -> void
{
  BinaryNotKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::BinarySubtract(const Image & source1, const Image & source2, const Image & destination) -> void
{
  BinarySubtractKernel kernel(this->GetDevice());
  kernel.SetInput1(source1);
  kernel.SetInput2(source2);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::BinaryXor(const Image & source1, const Image & source2, const Image & destination) -> void
{
  BinaryXorKernel kernel(this->GetDevice());
  kernel.SetInput1(source1);
  kernel.SetInput2(source2);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::SubtractImages(const Image & source1, const Image & source2, const Image & destination) -> void
{
  this->AddImagesWeighted(source1, source2, destination, 1, -1);
}

auto
Clesperanto::DilateSphere(const Image & source, const Image & destination) -> void
{
  DilateSphereKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::ErodeSphere(const Image & source, const Image & destination) -> void
{
  ErodeSphereKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::Equal(const Image & source1, const Image & source2, const Image & destination) -> void
{
  EqualKernel kernel(this->GetDevice());
  kernel.SetInput1(source1);
  kernel.SetInput2(source2);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::EqualConstant(const Image & source, const Image & destination, const float & scalar) -> void
{
  EqualConstantKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.SetConstant(scalar);
  kernel.Execute();
}

auto
Clesperanto::GaussianBlur(const Image & source,
                          const Image & destination,
                          const float & sigma_x,
                          const float & sigma_y,
                          const float & sigma_z) -> void
{
  GaussianBlurKernel_Call(this->GetDevice(), source, destination, sigma_x, sigma_y, sigma_z);
}

auto
Clesperanto::MaximumBox(const Image & source,
                        const Image & destination,
                        const int &   radius_x,
                        const int &   radius_y,
                        const int &   radius_z) -> void
{
  MaximumBoxKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.SetRadius(radius_x, radius_y, radius_z);
  kernel.Execute();
}

auto
Clesperanto::MinimumBox(const Image & source,
                        const Image & destination,
                        const int &   radius_x,
                        const int &   radius_y,
                        const int &   radius_z) -> void
{
  MinimumBoxKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.SetRadius(radius_x, radius_y, radius_z);
  kernel.Execute();
}

auto
Clesperanto::MeanBox(const Image & source,
                     const Image & destination,
                     const int &   radius_x,
                     const int &   radius_y,
                     const int &   radius_z) -> void
{
  MeanBoxKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.SetRadius(radius_x, radius_y, radius_z);
  kernel.Execute();
}

auto
Clesperanto::Greater(const Image & source1, const Image & source2, const Image & destination) -> void
{
  GreaterKernel kernel(this->GetDevice());
  kernel.SetInput1(source1);
  kernel.SetInput2(source2);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::GreaterOrEqual(const Image & source1, const Image & source2, const Image & destination) -> void
{
  GreaterOrEqualKernel kernel(this->GetDevice());
  kernel.SetInput1(source1);
  kernel.SetInput2(source2);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::GreaterConstant(const Image & source, const Image & destination, const float & scalar) -> void
{
  GreaterConstantKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.SetConstant(scalar);
  kernel.Execute();
}

auto
Clesperanto::GreaterOrEqualConstant(const Image & source, const Image & destination, const float & scalar) -> void
{
  GreaterOrEqualConstantKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.SetConstant(scalar);
  kernel.Execute();
}

auto
Clesperanto::Mask(const Image & source, const Image & t_mask, const Image & destination) -> void
{
  MaskKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetMask(t_mask);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::MaskedVoronoiLabeling(const Image & source, const Image & t_mask, const Image & destination) -> void
{
  MaskedVoronoiLabelingKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetMask(t_mask);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::MaximumZProjection(const Image & source, const Image & destination) -> void
{
  MaximumZProjectionKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::MaximumYProjection(const Image & source, const Image & destination) -> void
{
  MaximumYProjectionKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::MaximumXProjection(const Image & source, const Image & destination) -> void
{
  MaximumXProjectionKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::MaximumOfAllPixels(const Image & source, const Image & destination) -> void
{
  MaximumOfAllPixelsKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::MinimumZProjection(const Image & source, const Image & destination) -> void
{
  MinimumZProjectionKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::MinimumYProjection(const Image & source, const Image & destination) -> void
{
  MinimumYProjectionKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::MinimumXProjection(const Image & source, const Image & destination) -> void
{
  MinimumXProjectionKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::MinimumOfAllPixels(const Image & source, const Image & destination) -> void
{
  MinimumOfAllPixelsKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::DifferenceOfGaussian(const Image & source,
                                  const Image & destination,
                                  const float & sigma1_x,
                                  const float & sigma1_y,
                                  const float & sigma1_z,
                                  const float & sigma2_x,
                                  const float & sigma2_y,
                                  const float & sigma2_z) -> void
{
  DifferenceOfGaussianKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.SetSigma1(sigma1_x, sigma1_y, sigma1_z);
  kernel.SetSigma2(sigma2_x, sigma2_y, sigma2_z);
  kernel.Execute();
}

auto
Clesperanto::MeanSphere(const Image & source,
                        const Image & destination,
                        const float & radius_x,
                        const float & radius_y,
                        const float & radius_z) -> void
{
  MeanSphereKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.SetRadius(radius_x, radius_y, radius_z);
  kernel.Execute();
}

auto
Clesperanto::NonzeroMinimumBox(const Image & source, const Image & t_flag, const Image & destination) -> void
{
  NonzeroMinimumBoxKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.SetOutputFlag(t_flag);
  kernel.Execute();
}

auto
Clesperanto::NotEqual(const Image & source1, const Image & source2, const Image & destination) -> void
{
  NotEqualKernel kernel(this->GetDevice());
  kernel.SetInput1(source1);
  kernel.SetInput2(source2);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::NotEqualConstant(const Image & source, const Image & destination, const float & scalar) -> void
{
  NotEqualConstantKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.SetConstant(scalar);
  kernel.Execute();
}

auto
Clesperanto::Smaller(const Image & source1, const Image & source2, const Image & destination) -> void
{
  SmallerKernel kernel(this->GetDevice());
  kernel.SetInput1(source1);
  kernel.SetInput2(source2);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::SmallerOrEqual(const Image & source1, const Image & source2, const Image & destination) -> void
{
  SmallerOrEqualKernel kernel(this->GetDevice());
  kernel.SetInput1(source1);
  kernel.SetInput2(source2);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::SmallerConstant(const Image & source, const Image & destination, const float & scalar) -> void
{
  SmallerConstantKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.SetConstant(scalar);
  kernel.Execute();
}

auto
Clesperanto::SmallerOrEqualConstant(const Image & source, const Image & destination, const float & scalar) -> void
{
  SmallerOrEqualConstantKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.SetConstant(scalar);
  kernel.Execute();
}

auto
Clesperanto::Absolute(const Image & source, const Image & destination) -> void
{
  AbsoluteKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::Sobel(const Image & source, const Image & destination) -> void
{
  SobelKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::Set(const Image & source, const float & scalar) -> void
{
  SetKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetValue(scalar);
  kernel.Execute();
}

auto
Clesperanto::SetNonzeroPixelsToPixelindex(const Image & source, const Image & destination) -> void
{
  SetNonzeroPixelsToPixelindexKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.SetOffset(1);
  kernel.Execute();
}

auto
Clesperanto::DetectMaximaBox(const Image & source, const Image & destination) -> void
{
  DetectMaximaKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::Copy(const Image & source, const Image & destination) -> void
{
  CopyKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::SumZProjection(const Image & source, const Image & destination) -> void
{
  SumZProjectionKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::SumYProjection(const Image & source, const Image & destination) -> void
{
  SumYProjectionKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::SumXProjection(const Image & source, const Image & destination) -> void
{
  SumXProjectionKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::SumOfAllPixels(const Image & source, const Image & destination) -> void
{
  SumOfAllPixelsKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::ConnectedComponentLabelingBox(const Image & source, const Image & destination) -> void
{
  ConnectedComponentLabelingBoxKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::ReplaceIntensity(const Image & source,
                              const Image & destination,
                              const float & input_intensity,
                              const float & output_intensity) -> void
{
  ReplaceIntensityKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.SetInValue(input_intensity);
  kernel.SetOutValue(output_intensity);
  kernel.Execute();
}

auto
Clesperanto::ReplaceIntensities(const Image & source, const Image & intensity_map, const Image & destination) -> void
{
  ReplaceIntensitiesKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.SetMap(intensity_map);
  kernel.Execute();
}

auto
Clesperanto::SetColumn(const Image & source, const int & column_index, const float & scalar) -> void
{
  SetColumnKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetColumn(column_index);
  kernel.SetValue(scalar);
  kernel.Execute();
}

auto
Clesperanto::SumReductionX(const Image & source, const Image & destination, const int & block_size) -> void
{
  SumReductionXKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.SetBlocksize(block_size);
  kernel.Execute();
}

auto
Clesperanto::BlockEnumerate(const Image & source, const Image & sum, const Image & destination, const int & block_size)
  -> void
{
  BlockEnumerateKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetInputSums(sum);
  kernel.SetOutput(destination);
  kernel.SetBlocksize(block_size);
  kernel.Execute();
}

auto
Clesperanto::FlagExistingLabels(const Image & source, const Image & destination) -> void
{
  FlagExistingLabelsKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::CloseIndexGapsInLabelMap(const Image & source, const Image & destination, const int & block_size) -> void
{
  CloseIndexGapsInLabelMapKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.SetBlockSize(block_size);
  kernel.Execute();
}

auto
Clesperanto::Histogram(const Image & source,
                       const Image & destination,
                       const int &   bins,
                       const float & min_intensity,
                       const float & max_intensity) -> void
{
  HistogramKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.SetSteps(1, 1, 1);
  kernel.SetNumBins(bins);
  kernel.SetMinimumIntensity(min_intensity);
  kernel.SetMaximumIntensity(max_intensity);
  kernel.Execute();
}

auto
Clesperanto::ThresholdOtsu(const Image & source, const Image & destination) -> void
{
  ThresholdOtsuKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::OnlyzeroOverwriteMaximumBox(const Image & source, const Image & flag, const Image & destination) -> void
{
  OnlyzeroOverwriteMaximumBoxKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput1(flag);
  kernel.SetOutput2(destination);
  kernel.Execute();
}

auto
Clesperanto::OnlyzeroOverwriteMaximumDiamond(const Image & source, const Image & flag, const Image & destination)
  -> void
{
  OnlyzeroOverwriteMaximumDiamondKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput1(flag);
  kernel.SetOutput2(destination);
  kernel.Execute();
}

auto
Clesperanto::VoronoiOtsuLabeling(const Image & source,
                                 const Image & destination,
                                 const float & sigma_spot,
                                 const float & sigma_outline) -> void
{
  VoronoiOtsuLabelingKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.SetSpotSigma(sigma_spot);
  kernel.SetOutlineSigma(sigma_outline);
  kernel.Execute();
}

auto
Clesperanto::ExtendLabelingViaVoronoi(const Image & source, const Image & destination) -> void
{
  ExtendLabelingViaVoronoiKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::TopHatBox(const Image & source,
                       const Image & destination,
                       const int &   radius_x,
                       const int &   radius_y,
                       const int &   radius_z) -> void
{
  TopHatBoxKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.SetRadius(radius_x, radius_y, radius_z);
  kernel.Execute();
}

auto
Clesperanto::MultiplyImages(const Image & source1, const Image & source2, const Image & destination) -> void
{
  MultiplyImagesKernel kernel(this->GetDevice());
  kernel.SetInput1(source1);
  kernel.SetInput2(source2);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::DivideImages(const Image & source1, const Image & source2, const Image & destination) -> void
{
  DivideImagesKernel kernel(this->GetDevice());
  kernel.SetInput1(source1);
  kernel.SetInput2(source2);
  kernel.SetOutput(destination);
  kernel.Execute();
}

auto
Clesperanto::SubtractImageFromScalar(const Image & source, const Image & destination, const float & scalar) -> void
{
  SubtractImageFromScalarKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.SetScalar(scalar);
  kernel.Execute();
}

auto
Clesperanto::DilateLabels(const Image & source, const Image & destination, const int & radius) -> void
{
  DilateLabelsKernel kernel(this->GetDevice());
  kernel.SetInput(source);
  kernel.SetOutput(destination);
  kernel.SetRadius(radius);
  kernel.Execute();
}

} // namespace cle