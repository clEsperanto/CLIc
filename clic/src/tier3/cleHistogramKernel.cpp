
#include "cleHistogramKernel.hpp"

#include "cleMaximumOfAllPixelsKernel.hpp"
#include "cleMinimumOfAllPixelsKernel.hpp"
#include "cleSumZProjectionKernel.hpp"
#include "utils.hpp"

namespace cle
{

HistogramKernel::HistogramKernel(std::shared_ptr<GPU> t_gpu) :    
    Kernel( t_gpu, 
            "histogram", 
            {"src" , "dst", "minimum", "maximum", "step_size_x", "step_size_y", "step_size_z"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void HistogramKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void HistogramKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "histogram");
}

void HistogramKernel::SetMinimumIntensity(float t_min_intensity)
{
    this->m_MinIntensity = t_min_intensity;
}

void HistogramKernel::SetMaximumIntensity(float t_max_intensity)
{
    this->m_MaxIntensity = t_max_intensity;
}

void HistogramKernel::SetSteps(int t_step_x, int t_step_y, int t_step_z)
{
    this->AddObject(t_step_x, "step_size_x");
    this->AddObject(t_step_y, "step_size_y");
    this->AddObject(t_step_z, "step_size_z");
}

void HistogramKernel::SetNumBins(unsigned int t_bin)
{
    this->AddConstant(std::to_string(t_bin), "NUMBER_OF_HISTOGRAM_BINS");
}

void HistogramKernel::Execute()
{
    auto dst = this->GetParameter<Object>("histogram");
    auto src = this->GetParameter<Object>("src");
    size_t nb_bins = this->GetConstant("NUMBER_OF_HISTOGRAM_BINS");

    // set min-max intensity of src, if not provided.
    if(this->m_MinIntensity == std::numeric_limits<float>::infinity() ||
       this->m_MaxIntensity == std::numeric_limits<float>::infinity() )
    {
        auto temp_scalar_buffer = this->m_gpu->Create<float>({1,1,1});
        float temp_scalar_intensity;

        MinimumOfAllPixelsKernel minimum_intensity_kernel(this->m_gpu);
        minimum_intensity_kernel.SetInput(*src);
        minimum_intensity_kernel.SetOutput(temp_scalar_buffer);
        minimum_intensity_kernel.Execute();
        this->m_MinIntensity = this->m_gpu->Pull<float>(temp_scalar_buffer).front();

        MaximumOfAllPixelsKernel maximum_intensity_kernel(this->m_gpu);
        maximum_intensity_kernel.SetInput(*src);
        maximum_intensity_kernel.SetOutput(temp_scalar_buffer);
        maximum_intensity_kernel.Execute();
        this->m_MaxIntensity = this->m_gpu->Pull<float>(temp_scalar_buffer).front();
    }
    this->AddObject(this->m_MinIntensity, "minimum");
    this->AddObject(this->m_MaxIntensity, "maximum");

    // create partial histogram step
    size_t nb_temp_hist = src->Shape()[1];
    auto partial_hist = this->m_gpu->Create<float>({nb_bins,1,nb_temp_hist});
    this->AddObject(partial_hist, "dst");

    // run histogram kernel
    this->BuildProgramKernel();
    this->SetArguments();
    this->SetGlobalNDRange({nb_temp_hist,1,1});
    this->EnqueueKernel();

    // run projection
    SumZProjectionKernel sum(this->m_gpu);
    sum.SetInput(partial_hist);
    sum.SetOutput(*dst);
    sum.Execute();
}

} // namespace cle