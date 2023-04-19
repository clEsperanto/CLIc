#include "clesperanto.hpp"

#include "cleKernelList.hpp"
namespace cle
{

Clesperanto::Clesperanto()
  : device_(std::make_shared<Processor>())
{
  if (Processor::ListAvailableDevices().empty())
  {
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
Clesperanto::SelectDevice(const std::string & name, const std::string & type) -> void
{
  this->GetDevice()->SelectDevice(name, type);
}

auto
Clesperanto::Info() -> std::string
{
  return this->GetDevice()->GetDeviceInfo();
}

auto
Clesperanto::AddImageAndScalar(const Image & source, const Image & destination, const float & scalar) -> void
{
  AddImageAndScalarKernel_Call(this->GetDevice(), source, destination, scalar);
}

auto
Clesperanto::AddImagesWeighted(const Image & source1,
                               const Image & source2,
                               const Image & destination,
                               const float & weight1,
                               const float & weight2) -> void
{
  AddImagesWeightedKernel_Call(this->GetDevice(), source1, source2, destination, weight1, weight2);
}

auto
Clesperanto::AddImages(const Image & source1, const Image & source2, const Image & destination) -> void
{
  this->AddImagesWeighted(source1, source2, destination, 1, 1);
}

auto
Clesperanto::BinaryEdgeDetection(const Image & source, const Image & destination) -> void
{
  BinaryEdgeDetectionKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::BinaryAnd(const Image & source1, const Image & source2, const Image & destination) -> void
{
  BinaryAndKernel_Call(this->GetDevice(), source1, source2, destination);
}

auto
Clesperanto::BinaryOr(const Image & source1, const Image & source2, const Image & destination) -> void
{
  BinaryOrKernel_Call(this->GetDevice(), source1, source2, destination);
}

auto
Clesperanto::BinaryNot(const Image & source, const Image & destination) -> void
{
  BinaryNotKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::BinarySubtract(const Image & source1, const Image & source2, const Image & destination) -> void
{
  BinarySubtractKernel_Call(this->GetDevice(), source1, source2, destination);
}

auto
Clesperanto::BinaryXor(const Image & source1, const Image & source2, const Image & destination) -> void
{
  BinaryXorKernel_Call(this->GetDevice(), source1, source2, destination);
}

auto
Clesperanto::Convolve(const Image & source, const Image & convolve_kernel, const Image & destination) -> void
{
  ConvolveKernel_Call(this->GetDevice(), source, convolve_kernel, destination);
}

auto
Clesperanto::Crop(const Image & source,
                  const Image & destination,
                  const int &   index0,
                  const int &   index1,
                  const int &   index2) -> void
{
  CropKernel_Call(this->GetDevice(), source, destination, index0, index1, index2);
}

auto
Clesperanto::SubtractImages(const Image & source1, const Image & source2, const Image & destination) -> void
{
  this->AddImagesWeighted(source1, source2, destination, 1, -1);
}

auto
Clesperanto::DilateBox(const Image & source, const Image & destination) -> void
{
  DilateBoxKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::DilateSphere(const Image & source, const Image & destination) -> void
{
  DilateSphereKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::DivideImageAndScalar(const Image & source, const Image & destination, const float & scalar) -> void
{
  DivideImageAndScalarKernel_Call(this->GetDevice(), source, destination, scalar);
}

auto
Clesperanto::ErodeBox(const Image & source, const Image & destination) -> void
{
  ErodeBoxKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::ErodeSphere(const Image & source, const Image & destination) -> void
{
  ErodeSphereKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::Equal(const Image & source1, const Image & source2, const Image & destination) -> void
{
  EqualKernel_Call(this->GetDevice(), source1, source2, destination);
}

auto
Clesperanto::EqualConstant(const Image & source, const Image & destination, const float & scalar) -> void
{
  EqualConstantKernel_Call(this->GetDevice(), source, destination, scalar);
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
  MaximumBoxKernel_Call(this->GetDevice(), source, destination, radius_x, radius_y, radius_z);
}

auto
Clesperanto::MaximumSphere(const Image & source,
                           const Image & destination,
                           const float & radius_x,
                           const float & radius_y,
                           const float & radius_z) -> void
{
  MaximumSphereKernel_Call(this->GetDevice(), source, destination, radius_x, radius_y, radius_z);
}

auto
Clesperanto::MaximumImages(const Image & source1, const Image & source2, const Image & destination) -> void
{
  MaximumImagesKernel_Call(this->GetDevice(), source1, source2, destination);
}

auto
Clesperanto::MinimumImages(const Image & source1, const Image & source2, const Image & destination) -> void
{
  MinimumImagesKernel_Call(this->GetDevice(), source1, source2, destination);
}


auto
Clesperanto::MinimumBox(const Image & source,
                        const Image & destination,
                        const int &   radius_x,
                        const int &   radius_y,
                        const int &   radius_z) -> void
{
  MinimumBoxKernel_Call(this->GetDevice(), source, destination, radius_x, radius_y, radius_z);
}

auto
Clesperanto::MinimumSphere(const Image & source,
                           const Image & destination,
                           const float & radius_x,
                           const float & radius_y,
                           const float & radius_z) -> void
{
  MinimumSphereKernel_Call(this->GetDevice(), source, destination, radius_x, radius_y, radius_z);
}

auto
Clesperanto::MeanBox(const Image & source,
                     const Image & destination,
                     const int &   radius_x,
                     const int &   radius_y,
                     const int &   radius_z) -> void
{
  MeanBoxKernel_Call(this->GetDevice(), source, destination, radius_x, radius_y, radius_z);
}

auto
Clesperanto::GradientX(const Image & source, const Image & destination) -> void
{
  GradientXKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::GradientY(const Image & source, const Image & destination) -> void
{
  GradientYKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::GradientZ(const Image & source, const Image & destination) -> void
{
  GradientZKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::Greater(const Image & source1, const Image & source2, const Image & destination) -> void
{
  GreaterKernel_Call(this->GetDevice(), source1, source2, destination);
}

auto
Clesperanto::GreaterOrEqual(const Image & source1, const Image & source2, const Image & destination) -> void
{
  GreaterOrEqualKernel_Call(this->GetDevice(), source1, source2, destination);
}

auto
Clesperanto::GreaterConstant(const Image & source, const Image & destination, const float & scalar) -> void
{
  GreaterConstantKernel_Call(this->GetDevice(), source, destination, scalar);
}

auto
Clesperanto::GreaterOrEqualConstant(const Image & source, const Image & destination, const float & scalar) -> void
{
  GreaterOrEqualConstantKernel_Call(this->GetDevice(), source, destination, scalar);
}

auto
Clesperanto::LaplaceBox(const Image & source, const Image & destination) -> void
{
  LaplaceBoxKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::Mask(const Image & source, const Image & t_mask, const Image & destination) -> void
{
  MaskKernel_Call(this->GetDevice(), source, t_mask, destination);
}

auto
Clesperanto::MaskedVoronoiLabeling(const Image & source, const Image & t_mask, const Image & destination) -> void
{
  MaskedVoronoiLabelingKernel_Call(this->GetDevice(), source, destination, t_mask);
}

auto
Clesperanto::MaximumZProjection(const Image & source, const Image & destination) -> void
{
  MaximumZProjectionKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::MaximumYProjection(const Image & source, const Image & destination) -> void
{
  MaximumYProjectionKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::MaximumXProjection(const Image & source, const Image & destination) -> void
{
  MaximumXProjectionKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::MaximumOfAllPixels(const Image & source, const Image & destination) -> void
{
  MaximumOfAllPixelsKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::MinimumZProjection(const Image & source, const Image & destination) -> void
{
  MinimumZProjectionKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::MinimumYProjection(const Image & source, const Image & destination) -> void
{
  MinimumYProjectionKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::MinimumXProjection(const Image & source, const Image & destination) -> void
{
  MinimumXProjectionKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::MinimumOfAllPixels(const Image & source, const Image & destination) -> void
{
  MinimumOfAllPixelsKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::MultiplyImageAndScalar(const Image & source, const Image & destination, const float & scalar) -> void
{
  MultiplyImageAndScalarKernel_Call(this->GetDevice(), source, destination, scalar);
}

auto
Clesperanto::Power(const Image & source, const Image & destination, const float & exponent) -> void
{
  PowerKernel_Call(this->GetDevice(), source, destination, exponent);
}

auto
Clesperanto::PowerImages(const Image & source1, const Image & source2, const Image & destination) -> void
{
  PowerImagesKernel_Call(this->GetDevice(), source1, source2, destination);
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
  DifferenceOfGaussianKernel_Call(
    this->GetDevice(), source, destination, sigma1_x, sigma1_y, sigma1_z, sigma2_x, sigma2_y, sigma2_z);
}

auto
Clesperanto::MeanSphere(const Image & source,
                        const Image & destination,
                        const float & radius_x,
                        const float & radius_y,
                        const float & radius_z) -> void
{
  MeanSphereKernel_Call(this->GetDevice(), source, destination, radius_x, radius_y, radius_z);
}

auto
Clesperanto::NonzeroMinimumBox(const Image & source, const Image & t_flag, const Image & destination) -> void
{
  NonzeroMinimumBoxKernel_Call(this->GetDevice(), source, destination, t_flag);
}

auto
Clesperanto::NotEqual(const Image & source1, const Image & source2, const Image & destination) -> void
{
  NotEqualKernel_Call(this->GetDevice(), source1, source2, destination);
}

auto
Clesperanto::NotEqualConstant(const Image & source, const Image & destination, const float & scalar) -> void
{
  NotEqualConstantKernel_Call(this->GetDevice(), source, destination, scalar);
}

auto
Clesperanto::Smaller(const Image & source1, const Image & source2, const Image & destination) -> void
{
  SmallerKernel_Call(this->GetDevice(), source1, source2, destination);
}

auto
Clesperanto::SmallerOrEqual(const Image & source1, const Image & source2, const Image & destination) -> void
{
  SmallerOrEqualKernel_Call(this->GetDevice(), source1, source2, destination);
}

auto
Clesperanto::SmallerConstant(const Image & source, const Image & destination, const float & scalar) -> void
{
  SmallerConstantKernel_Call(this->GetDevice(), source, destination, scalar);
}

auto
Clesperanto::SmallerOrEqualConstant(const Image & source, const Image & destination, const float & scalar) -> void
{
  SmallerOrEqualConstantKernel_Call(this->GetDevice(), source, destination, scalar);
}

auto
Clesperanto::Absolute(const Image & source, const Image & destination) -> void
{
  AbsoluteKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::Sobel(const Image & source, const Image & destination) -> void
{
  SobelKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::Set(const Image & source, const float & scalar) -> void
{
  SetKernel_Call(this->GetDevice(), source, scalar);
}

auto
Clesperanto::SetNonzeroPixelsToPixelindex(const Image & source, const Image & destination) -> void
{
  SetNonzeroPixelsToPixelindexKernel_Call(this->GetDevice(), source, destination, 1);
}

auto
Clesperanto::DetectMaximaBox(const Image & source, const Image & destination) -> void
{
  DetectMaximaKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::Copy(const Image & source, const Image & destination) -> void
{
  CopyKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::SumZProjection(const Image & source, const Image & destination) -> void
{
  SumZProjectionKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::SumYProjection(const Image & source, const Image & destination) -> void
{
  SumYProjectionKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::SumXProjection(const Image & source, const Image & destination) -> void
{
  SumXProjectionKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::SumOfAllPixels(const Image & source, const Image & destination) -> void
{
  SumOfAllPixelsKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::ConnectedComponentLabelingBox(const Image & source, const Image & destination) -> void
{
  ConnectedComponentLabelingBoxKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::ReplaceIntensity(const Image & source,
                              const Image & destination,
                              const float & input_intensity,
                              const float & output_intensity) -> void
{
  ReplaceIntensityKernel_Call(this->GetDevice(), source, destination, input_intensity, output_intensity);
}

auto
Clesperanto::ReplaceIntensities(const Image & source, const Image & intensity_map, const Image & destination) -> void
{
  ReplaceIntensitiesKernel_Call(this->GetDevice(), source, destination, intensity_map);
}

auto
Clesperanto::SetColumn(const Image & source, const int & column_index, const float & scalar) -> void
{
  SetColumnKernel_Call(this->GetDevice(), source, column_index, scalar);
}

auto
Clesperanto::SumReductionX(const Image & source, const Image & destination, const int & block_size) -> void
{
  SumReductionXKernel_Call(this->GetDevice(), source, destination, block_size);
}

auto
Clesperanto::BlockEnumerate(const Image & source, const Image & sum, const Image & destination, const int & block_size)
  -> void
{
  BlockEnumerateKernel_Call(this->GetDevice(), source, sum, destination, block_size);
}

auto
Clesperanto::FlagExistingLabels(const Image & source, const Image & destination) -> void
{
  FlagExistingLabelsKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::CloseIndexGapsInLabelMap(const Image & source, const Image & destination, const int & block_size) -> void
{
  CloseIndexGapsInLabelMapKernel_Call(this->GetDevice(), source, destination, block_size);
}

auto
Clesperanto::Histogram(const Image & source,
                       const Image & destination,
                       const int &   bins,
                       const float & min_intensity,
                       const float & max_intensity) -> void
{
  HistogramKernel_Call(this->GetDevice(), source, destination, bins, min_intensity, max_intensity);
}

auto
Clesperanto::ThresholdOtsu(const Image & source, const Image & destination) -> void
{
  ThresholdOtsuKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::OnlyzeroOverwriteMaximumBox(const Image & source, const Image & flag, const Image & destination) -> void
{
  OnlyzeroOverwriteMaximumBoxKernel_Call(this->GetDevice(), source, flag, destination);
}

auto
Clesperanto::OnlyzeroOverwriteMaximumDiamond(const Image & source, const Image & flag, const Image & destination)
  -> void
{
  OnlyzeroOverwriteMaximumDiamondKernel_Call(this->GetDevice(), source, flag, destination);
}

auto
Clesperanto::VoronoiOtsuLabeling(const Image & source,
                                 const Image & destination,
                                 const float & sigma_spot,
                                 const float & sigma_outline) -> void
{
  VoronoiOtsuLabelingKernel_Call(this->GetDevice(), source, destination, sigma_spot, sigma_outline);
}

auto
Clesperanto::ExtendLabelingViaVoronoi(const Image & source, const Image & destination) -> void
{
  ExtendLabelingViaVoronoiKernel_Call(this->GetDevice(), source, destination);
}

auto
Clesperanto::TopHatBox(const Image & source,
                       const Image & destination,
                       const int &   radius_x,
                       const int &   radius_y,
                       const int &   radius_z) -> void
{
  TopHatBoxKernel_Call(this->GetDevice(), source, destination, radius_x, radius_y, radius_z);
}

auto
Clesperanto::MultiplyImages(const Image & source1, const Image & source2, const Image & destination) -> void
{
  MultiplyImagesKernel_Call(this->GetDevice(), source1, source2, destination);
}

auto
Clesperanto::DivideImages(const Image & source1, const Image & source2, const Image & destination) -> void
{
  DivideImagesKernel_Call(this->GetDevice(), source1, source2, destination);
}

auto
Clesperanto::SubtractImageFromScalar(const Image & source, const Image & destination, const float & scalar) -> void
{
  SubtractImageFromScalarKernel_Call(this->GetDevice(), source, destination, scalar);
}

auto
Clesperanto::DilateLabels(const Image & source, const Image & destination, const int & radius) -> void
{
  DilateLabelsKernel_Call(this->GetDevice(), source, destination, radius);
}

} // namespace cle