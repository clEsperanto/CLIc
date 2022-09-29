
#include "cleVoronoiOtsuLabelingKernel.hpp"

#include "cleBinaryAndKernel.hpp"
#include "cleDetectMaximaKernel.hpp"
#include "cleGaussianBlurKernel.hpp"
#include "cleMaskKernel.hpp"
#include "cleMaskedVoronoiLabelingKernel.hpp"
#include "cleMemory.hpp"
#include "cleThresholdOtsuKernel.hpp"


namespace cle
{

VoronoiOtsuLabelingKernel::VoronoiOtsuLabelingKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{}

auto
VoronoiOtsuLabelingKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
VoronoiOtsuLabelingKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
VoronoiOtsuLabelingKernel::SetSpotSigma(const float & sigma) -> void
{
  this->spot_sigma_ = sigma;
}

auto
VoronoiOtsuLabelingKernel::SetOutlineSigma(const float & sigma) -> void
{
  this->output_sigma_ = sigma;
}

auto
VoronoiOtsuLabelingKernel::Execute() -> void
{
  auto temp_data_type = FLOAT;
  auto src = this->GetImage("src");
  auto dst = this->GetImage("dst");

  auto temp = Memory::AllocateMemory(this->GetDevice(), src->Shape(), temp_data_type, src->GetMemoryType());
  GaussianBlurKernel gaussianSpot(this->GetDevice());
  gaussianSpot.SetInput(*src);
  gaussianSpot.SetOutput(temp);
  gaussianSpot.SetSigma(spot_sigma_, spot_sigma_, spot_sigma_);
  gaussianSpot.Execute();

  auto               spot = Memory::AllocateMemory(temp);
  DetectMaximaKernel maxima(this->GetDevice());
  maxima.SetInput(temp);
  maxima.SetOutput(spot);
  maxima.Execute();

  GaussianBlurKernel gaussianOutline(this->GetDevice());
  gaussianOutline.SetInput(*src);
  gaussianOutline.SetOutput(temp);
  gaussianOutline.SetSigma(output_sigma_, output_sigma_, output_sigma_);
  gaussianOutline.Execute();

  auto                segmentation = Memory::AllocateMemory(temp);
  ThresholdOtsuKernel threshold(this->GetDevice());
  threshold.SetInput(temp);
  threshold.SetOutput(segmentation);
  threshold.Execute();

  auto            binary = Memory::AllocateMemory(temp);
  BinaryAndKernel bin(this->GetDevice());
  bin.SetInput1(segmentation);
  bin.SetInput2(spot);
  bin.SetOutput(binary);
  bin.Execute();

  MaskedVoronoiLabelingKernel labeling(this->GetDevice());
  labeling.SetInput(binary);
  labeling.SetMask(segmentation);
  labeling.SetOutput(temp);
  labeling.Execute();

  MaskKernel mask(this->GetDevice());
  mask.SetInput(temp);
  mask.SetMask(segmentation);
  mask.SetOutput(*dst);
  mask.Execute();
}

} // namespace cle
