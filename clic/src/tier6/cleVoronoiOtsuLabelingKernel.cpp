
#include "cleVoronoiOtsuLabelingKernel.hpp"

#include "cleBinaryAndKernel.hpp"
#include "cleDetectMaximaKernel.hpp"
#include "cleGaussianBlurKernel.hpp"
#include "cleMaskKernel.hpp"
#include "cleMaskedVoronoiLabelingKernel.hpp"
#include "cleMemory.hpp"
#include "cleThresholdOtsuKernel.hpp"
#include "cleUtils.hpp"

namespace cle
{

VoronoiOtsuLabelingKernel::VoronoiOtsuLabelingKernel (const ProcessorPointer &device) : Operation (device, 2)
{
}

void
VoronoiOtsuLabelingKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
VoronoiOtsuLabelingKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
VoronoiOtsuLabelingKernel::SetSpotSigma (const float &sigma)
{
    this->spot_sigma_ = sigma;
}

void
VoronoiOtsuLabelingKernel::SetOutlineSigma (const float &sigma)
{
    this->output_sigma_ = sigma;
}

void
VoronoiOtsuLabelingKernel::Execute ()
{
    auto temp_data_type = CL_FLOAT;
    auto src = this->GetImage ("src");
    auto dst = this->GetImage ("dst");

    auto temp = Memory::AllocateObject (this->Device (), src->Shape (), temp_data_type, src->MemType ().Get ());
    GaussianBlurKernel gaussianSpot (this->Device ());
    gaussianSpot.SetInput (*src);
    gaussianSpot.SetOutput (temp);
    gaussianSpot.SetSigma (spot_sigma_, spot_sigma_, spot_sigma_);
    gaussianSpot.Execute ();

    auto spot = Memory::AllocateObject (temp);
    DetectMaximaKernel maxima (this->Device ());
    maxima.SetInput (temp);
    maxima.SetOutput (spot);
    maxima.Execute ();

    GaussianBlurKernel gaussianOutline (this->Device ());
    gaussianOutline.SetInput (*src);
    gaussianOutline.SetOutput (temp);
    gaussianOutline.SetSigma (output_sigma_, output_sigma_, output_sigma_);
    gaussianOutline.Execute ();

    auto segmentation = Memory::AllocateObject (temp);
    ThresholdOtsuKernel threshold (this->Device ());
    threshold.SetInput (temp);
    threshold.SetOutput (segmentation);
    threshold.Execute ();

    auto binary = Memory::AllocateObject (temp);
    BinaryAndKernel bin (this->Device ());
    bin.SetInput1 (segmentation);
    bin.SetInput2 (spot);
    bin.SetOutput (binary);
    bin.Execute ();

    MaskedVoronoiLabelingKernel labeling (this->Device ());
    labeling.SetInput (binary);
    labeling.SetMask (segmentation);
    labeling.SetOutput (temp);
    labeling.Execute ();

    MaskKernel mask (this->Device ());
    mask.SetInput (temp);
    mask.SetMask (segmentation);
    mask.SetOutput (*dst);
    mask.Execute ();
}

} // namespace cle
