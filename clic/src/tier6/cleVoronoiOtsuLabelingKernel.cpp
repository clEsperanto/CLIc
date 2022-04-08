
#include "cleVoronoiOtsuLabelingKernel.hpp"

#include "cleGaussianBlurKernel.hpp"
#include "cleDetectMaximaKernel.hpp"
#include "cleThresholdOtsuKernel.hpp"
#include "cleBinaryAndKernel.hpp"
#include "cleMaskedVoronoiLabelingKernel.hpp"
#include "cleMaskKernel.hpp"

#include "utils.hpp"


namespace cle
{

VoronoiOtsuLabelingKernel::VoronoiOtsuLabelingKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "voronoi_otsu_labeling",
            {"src" , "dst", "scalar0", "scalar1"}
    )
{}    

void VoronoiOtsuLabelingKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void VoronoiOtsuLabelingKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void VoronoiOtsuLabelingKernel::SetSpotSigma(float t_x)
{
    this->m_SpotSigma = t_x;
}

void VoronoiOtsuLabelingKernel::SetOutlineSigma(float t_x)
{
    this->m_OutlineSigma = t_x;
}

void VoronoiOtsuLabelingKernel::Execute()
{
    // get I/O pointers
    auto src = this->GetParameter<Object>("src");
    auto dst = this->GetParameter<Object>("dst");

    auto temp = this->m_gpu->Create<float>(src->Shape());
    GaussianBlurKernel gaussianSpot(this->m_gpu);
    gaussianSpot.SetInput(*src);
    gaussianSpot.SetOutput(temp);
    gaussianSpot.SetSigma(m_SpotSigma, m_SpotSigma, m_SpotSigma);
    gaussianSpot.Execute();

    auto spot = this->m_gpu->Create<float>(src->Shape());
    DetectMaximaKernel maxima(this->m_gpu);
    maxima.SetInput(temp);
    maxima.SetOutput(spot);
    maxima.Execute();

    GaussianBlurKernel gaussianOutline(this->m_gpu);
    gaussianOutline.SetInput(*src);
    gaussianOutline.SetOutput(temp);
    gaussianOutline.SetSigma(m_OutlineSigma, m_OutlineSigma, m_OutlineSigma);
    gaussianOutline.Execute();

    auto segmentation = this->m_gpu->Create<float>(src->Shape());
    ThresholdOtsuKernel threshold(this->m_gpu);
    threshold.SetInput(temp);
    threshold.SetOutput(segmentation);
    threshold.Execute();

    auto binary = this->m_gpu->Create<float>(src->Shape());
    BinaryAndKernel bin(this->m_gpu);
    bin.SetInput1(segmentation);
    bin.SetInput2(spot);
    bin.SetOutput(binary);
    bin.Execute();

    MaskedVoronoiLabelingKernel labeling(this->m_gpu);
    labeling.SetInput(binary);
    labeling.SetMask(segmentation);
    labeling.SetOutput(temp);
    labeling.Execute();

    MaskKernel mask(this->m_gpu);
    mask.SetInput(temp);
    mask.SetMask(segmentation);
    mask.SetOutput(*dst);
    mask.Execute();
}

} // namespace cle
