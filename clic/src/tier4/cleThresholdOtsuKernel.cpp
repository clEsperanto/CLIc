
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
  auto               src = this->GetImage("src");
  auto               dst = this->GetImage("dst");
  const size_t       bin = 256;
  std::vector<float> hist_array(bin);

  // compute inputs min / max intensity
  // * should be removed because already defined in histogram class ?
  //
  float                    min_intensity = 0;
  float                    max_intensity = 0;
  auto                     temp_scalar_buffer = Memory::AllocateMemory(this->GetDevice(), { 1, 1, 1 });
  MinimumOfAllPixelsKernel minimum_intensity_kernel(this->GetDevice());
  minimum_intensity_kernel.SetInput(*src);
  minimum_intensity_kernel.SetOutput(temp_scalar_buffer);
  minimum_intensity_kernel.Execute();
  Memory::ReadObject<float>(temp_scalar_buffer, min_intensity);
  MaximumOfAllPixelsKernel maximum_intensity_kernel(this->GetDevice());
  maximum_intensity_kernel.SetInput(*src);
  maximum_intensity_kernel.SetOutput(temp_scalar_buffer);
  maximum_intensity_kernel.Execute();
  Memory::ReadObject<float>(temp_scalar_buffer, max_intensity);

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
  Memory::ReadObject<float>(hist, hist_array.data(), hist_array.size());

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
  for (auto intensity = 0; intensity < range.size(); ++intensity)
  {
    range[intensity] = static_cast<float>(intensity) * (max_intensity - min_intensity) / (bin - 1) + min_intensity;
    sum_1 += range[intensity] * hist_array[intensity];
  }
  for (auto intensity = 0; intensity < range.size(); ++intensity)
  {
    if (hist_array[intensity] == 0)
    {
      continue;
    }
    weight_1 += hist_array[intensity];
    weight_2 = nb_pixels - weight_1;
    sum_2 += range[intensity] * hist_array[intensity];
    mean_1 = sum_2 / weight_1;
    mean_2 = (sum_1 - sum_2) / weight_2;
    variance = weight_1 * weight_2 * ((mean_1 - mean_2) * (mean_1 - mean_2));
    if (variance > max_variance)
    {
      threshold = range[intensity];
      max_variance = variance;
    }
  }
  if (src->GetDataType() != DataType::FLOAT32)
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
