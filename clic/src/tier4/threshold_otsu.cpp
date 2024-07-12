#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"

#include "utils.hpp"
#include <numeric>

namespace cle::tier4
{

auto
threshold_otsu_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  constexpr int bin = 256;
  const float   min_intensity = tier2::minimum_of_all_pixels_func(device, src);
  const float   max_intensity = tier2::maximum_of_all_pixels_func(device, src);
  auto          hist_array = Array::create(bin, 1, 1, 1, dType::FLOAT, mType::BUFFER, src->device());
  tier3::histogram_func(device, src, hist_array, bin, min_intensity, max_intensity);
  std::vector<float> histogram_array(hist_array->size());
  hist_array->readTo(histogram_array.data());
  float       threshold = -1;
  float       max_variance = -1;
  float       variance = 0;
  float       sum_1 = 0;
  float       sum_2 = 0;
  float       weight_1 = 0;
  float       weight_2 = 0;
  float       mean_1 = 0;
  float       mean_2 = 0;
  const float nb_pixels = src->size();
  const float intensity_factor = (max_intensity - min_intensity) / (bin - 1);

  std::vector<float> range(histogram_array.size());
  std::iota(range.begin(), range.end(), 0.0f);
  std::transform(range.begin(), range.end(), range.begin(), [intensity_factor, min_intensity](float intensity) {
    return intensity * intensity_factor + min_intensity;
  });
  sum_1 = std::transform_reduce(
    range.begin(), range.end(), histogram_array.begin(), 0.0f, std::plus<>(), [](float intensity, float hist_value) {
      return intensity * hist_value;
    });
  for (size_t index = 0; index < range.size(); ++index)
  {
    if (histogram_array[index] == 0)
    {
      continue;
    }
    weight_1 += histogram_array[index];
    weight_2 = nb_pixels - weight_1;
    sum_2 += range[index] * histogram_array[index];
    mean_1 = sum_2 / weight_1;
    mean_2 = (sum_1 - sum_2) / weight_2;
    variance = weight_1 * weight_2 * ((mean_1 - mean_2) * (mean_1 - mean_2));
    if (variance > max_variance)
    {
      threshold = range[index];
      max_variance = variance;
    }
  }
  tier0::create_like(src, dst, dType::BINARY);
  return tier1::greater_constant_func(device, src, dst, threshold);
}

} // namespace cle::tier4
