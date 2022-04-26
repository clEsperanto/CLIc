
#include "cleThresholdOtsuKernel.hpp"

#include "cleMinimumOfAllPixelsKernel.hpp"
#include "cleMaximumOfAllPixelsKernel.hpp"
#include "cleGreaterConstantKernel.hpp"
#include "cleHistogramKernel.hpp"

#include <math.h>
#include "utils.hpp"

namespace cle
{

ThresholdOtsuKernel::ThresholdOtsuKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "threshold_otsu",
            {"src" , "dst"}
    )
{}    

void ThresholdOtsuKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void ThresholdOtsuKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void ThresholdOtsuKernel::Execute()
{
    // get I/O pointers
    auto src = this->GetParameter<Object>("src");
    auto dst = this->GetParameter<Object>("dst");
    size_t bins = 256;

    // compute inputs min / max intensity
    // ? can be removed because already defined in histogram class
    auto temp_scalar_buffer = this->m_gpu->Create<float>({1,1,1});
    MinimumOfAllPixelsKernel minimum_intensity_kernel(this->m_gpu);
    minimum_intensity_kernel.SetInput(*src);
    minimum_intensity_kernel.SetOutput(temp_scalar_buffer);
    minimum_intensity_kernel.Execute();
    float min_intensity = this->m_gpu->Pull<float>(temp_scalar_buffer).front();
    MaximumOfAllPixelsKernel maximum_intensity_kernel(this->m_gpu);
    maximum_intensity_kernel.SetInput(*src);
    maximum_intensity_kernel.SetOutput(temp_scalar_buffer);
    maximum_intensity_kernel.Execute();
    float max_intensity = this->m_gpu->Pull<float>(temp_scalar_buffer).front();

    // compute src histogram
    auto hist = this->m_gpu->Create<float>({bins,1,1});
    HistogramKernel histogram(this->m_gpu);
    histogram.SetInput(*src);
    histogram.SetOutput(hist);
    histogram.SetSteps(1, 1, 1);
    histogram.SetNumBins(bins);
    histogram.SetMinimumIntensity(min_intensity);
    histogram.SetMaximumIntensity(max_intensity);
    histogram.Execute();
    auto hist_array = this->m_gpu->Pull<float>(hist);

    // compute otsu threshold value from histogram
    float threshold (-1), max_variance (-1), variance (0), sum_1 (0), sum_2 (0), weight_1 (0), weight_2 (0), mean_1 (0), mean_2 (0);
    float nb_pixels = src->Size();
    std::vector<float> range (hist_array.size());
    for(auto i = 0; i < range.size(); ++i)
    {
        range[i] = i * (max_intensity - min_intensity) / (bins-1) + min_intensity;
        sum_1 +=  range[i] * hist_array[i];
    }
    for(auto i = 0; i < range.size(); ++i)
    {
        if(hist_array[i] == 0) { continue; }
        weight_1 += hist_array[i];
        weight_2 = nb_pixels - weight_1;
        sum_2 += range[i] * hist_array[i];
        mean_1 = sum_2 / weight_1;
        mean_2 = (sum_1 - sum_2) / weight_2;
        variance = weight_1*weight_2*((mean_1-mean_2)*(mean_1-mean_2));
        if(variance > max_variance)
        {
            threshold = range[i];
            max_variance = variance;
        }
    }

    if (src->Type() != Object::DataType::FLOAT)
    {
        threshold = round(threshold);
    }

    // apply threshold value
    GreaterConstantKernel greater(this->m_gpu);
    greater.SetInput(*src);
    greater.SetOutput(*dst);
    greater.SetScalar(threshold);
    greater.Execute();
}

} // namespace cle
