
#include "cleThresholdOtsuKernel.hpp"

#include "cleGreaterConstantKernel.hpp"
#include "cleHistogramKernel.hpp"
#include "cleMaximumOfAllPixelsKernel.hpp"
#include "cleMemory.hpp"
#include "cleMinimumOfAllPixelsKernel.hpp"

#include <math.h>

namespace cle
{

ThresholdOtsuKernel::ThresholdOtsuKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{}

auto
ThresholdOtsuKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
ThresholdOtsuKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
ThresholdOtsuKernel::Execute() -> void
{
  auto         src = this->GetImage("src");
  auto         dst = this->GetImage("dst");
  const size_t bin = 256;

  // compute inputs min / max intensity
  // * should be removed because already defined in histogram class ?
  //
  auto                     temp_scalar_buffer = Memory::AllocateMemory(this->GetDevice(), { 1, 1, 1 });
  MinimumOfAllPixelsKernel minimum_intensity_kernel(this->GetDevice());
  minimum_intensity_kernel.SetInput(*src);
  minimum_intensity_kernel.SetOutput(temp_scalar_buffer);
  minimum_intensity_kernel.Execute();
  float                    min_intensity = Memory::ReadObject<float>(temp_scalar_buffer).front();
  MaximumOfAllPixelsKernel maximum_intensity_kernel(this->GetDevice());
  maximum_intensity_kernel.SetInput(*src);
  maximum_intensity_kernel.SetOutput(temp_scalar_buffer);
  maximum_intensity_kernel.Execute();
  float max_intensity = Memory::ReadObject<float>(temp_scalar_buffer).front();

  // compute src histogram
  auto            hist = Memory::AllocateMemory(this->GetDevice(), { bin, 1, 1 });
  HistogramKernel histogram(this->GetDevice());
  histogram.SetInput(*src);
  histogram.SetOutput(hist);
  histogram.SetSteps(1, 1, 1);
  histogram.SetNumBins(bin);
  histogram.SetMinimumIntensity(min_intensity);
  histogram.SetMaximumIntensity(max_intensity);
  histogram.Execute();
  auto hist_array = Memory::ReadObject<float>(hist);

  // compute otsu threshold value from histogram
  float              threshold = -1;
  float              max_variance = -1;
  float              variance = 0;
  float              sum_1 = 0;
  float              sum_2 = 0;
  float              weight_1 = 0;
  float              weight_2 = 0;
  float              mean_1 = 0;
  float              mean_2 = 0;
  float              nb_pixels = src->Shape()[0] * src->Shape()[1] * src->Shape()[2];
  std::vector<float> range(hist_array.size());
  for (auto i = 0; i < range.size(); ++i)
  {
    range[i] = i * (max_intensity - min_intensity) / (bin - 1) + min_intensity;
    sum_1 += range[i] * hist_array[i];
  }
  for (auto i = 0; i < range.size(); ++i)
  {
    if (hist_array[i] == 0)
    {
      continue;
    }
    weight_1 += hist_array[i];
    weight_2 = nb_pixels - weight_1;
    sum_2 += range[i] * hist_array[i];
    mean_1 = sum_2 / weight_1;
    mean_2 = (sum_1 - sum_2) / weight_2;
    variance = weight_1 * weight_2 * ((mean_1 - mean_2) * (mean_1 - mean_2));
    if (variance > max_variance)
    {
      threshold = range[i];
      max_variance = variance;
    }
  }
  if (src->GetDataType() != FLOAT)
  {
    threshold = round(threshold);
  }

  // Apply threshold
  GreaterConstantKernel greater(this->GetDevice());
  greater.SetInput(*src);
  greater.SetOutput(*dst);
  greater.SetConstant(threshold);
  greater.Execute();
}

} // namespace cle
