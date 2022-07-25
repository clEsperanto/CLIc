
#include "cleVoronoiOtsuLabelingKernel.hpp"

#include "cleBinaryAndKernel.hpp"
#include "cleDetectMaximaKernel.hpp"
#include "cleGaussianBlurKernel.hpp"
#include "cleMaskKernel.hpp"
#include "cleMaskedVoronoiLabelingKernel.hpp"
#include "cleThresholdOtsuKernel.hpp"

#include "cleUtils.hpp"

namespace cle
{

VoronoiOtsuLabelingKernel::VoronoiOtsuLabelingKernel (const ProcessorPointer &device) : Operation (device, 4)
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
    // // get I/O pointers
    // auto src = this->GetParameter<Object> ("src");
    // auto dst = this->GetParameter<Object> ("dst");

    // auto temp = this->m_gpu->Create<float> (src->Shape ());
    // GaussianBlurKernel gaussianSpot (this->m_gpu);
    // gaussianSpot.SetInput (*src);
    // gaussianSpot.SetOutput (temp);
    // gaussianSpot.SetSigma (spot_sigma_, spot_sigma_, spot_sigma_);
    // gaussianSpot.Execute ();

    // auto spot = this->m_gpu->Create<float> (src->Shape ());
    // DetectMaximaKernel maxima (this->m_gpu);
    // maxima.SetInput (temp);
    // maxima.SetOutput (spot);
    // maxima.Execute ();

    // GaussianBlurKernel gaussianOutline (this->m_gpu);
    // gaussianOutline.SetInput (*src);
    // gaussianOutline.SetOutput (temp);
    // gaussianOutline.SetSigma (output_sigma_, output_sigma_, output_sigma_);
    // gaussianOutline.Execute ();

    // auto segmentation = this->m_gpu->Create<float> (src->Shape ());
    // ThresholdOtsuKernel threshold (this->m_gpu);
    // threshold.SetInput (temp);
    // threshold.SetOutput (segmentation);
    // threshold.Execute ();

    // auto binary = this->m_gpu->Create<float> (src->Shape ());
    // BinaryAndKernel bin (this->m_gpu);
    // bin.SetInput1 (segmentation);
    // bin.SetInput2 (spot);
    // bin.SetOutput (binary);
    // bin.Execute ();

    // MaskedVoronoiLabelingKernel labeling (this->m_gpu);
    // labeling.SetInput (binary);
    // labeling.SetMask (segmentation);
    // labeling.SetOutput (temp);
    // labeling.Execute ();

    // MaskKernel mask (this->m_gpu);
    // mask.SetInput (temp);
    // mask.SetMask (segmentation);
    // mask.SetOutput (*dst);
    // mask.Execute ();
}

} // namespace cle
