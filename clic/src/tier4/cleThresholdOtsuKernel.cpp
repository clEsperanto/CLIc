
#include "cleThresholdOtsuKernel.hpp"

#include "cleMinimumOfAllPixelsKernel.hpp"
#include "cleMaximumOfAllPixelsKernel.hpp"
#include "cleGreaterConstantKernel.hpp"
#include "cleHistogramKernel.hpp"

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
    //! currently not adapted to input data type and only computed on float
    //! this can lead to rounding error
    //! [1,2,3,4,5,6,7,8,9,10] -> T=5 (int), T=4.9 (float)
    float threshold (0), variance (0), max_variance (0), sum_1 (0), sum_2 (0), weight_1 (0), weight_2 (0), mean_1 (0), mean_2 (0);
    std::vector<float> range (hist_array.size());
    for(auto i = 0; i < range.size(); ++i)
    {
        range[i] = i * (max_intensity - min_intensity) / (range.size()-1) + min_intensity;
    }
    for(auto i = 0; i < range.size(); ++i)
    {
        sum_1 += range[i] * hist_array[i];
    }
    auto range_end = range.end();
    auto hist_end = hist_array.end();
    for(auto it_range = range.begin(), it_hist = hist_array.begin(); 
             it_range != range_end && it_hist != hist_end; ++it_range, ++it_hist)
    {
        weight_1 += *it_hist;
        if(weight_1 == 0)
        {
            continue;
        }
        weight_2 = src->Size() - weight_1;

        sum_2 += (*it_range) * (*it_hist);
        mean_1 = sum_2 / weight_1;
        mean_2 = (sum_1 - sum_2) / weight_2;

        variance = weight_1*weight_2*((mean_1-mean_2)*(mean_1-mean_2));
        if(variance > max_variance)
        {
            threshold = *it_range;
            max_variance = variance;
        }
    }

    // apply threshold value
    GreaterConstantKernel greater(this->m_gpu);
    greater.SetInput(*src);
    greater.SetOutput(*dst);
    greater.SetScalar(threshold);
    greater.Execute();
}

} // namespace cle
