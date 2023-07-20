#include "tier4.hpp"
#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"

#include <numeric>

namespace cle::tier4
{

auto
relabel_sequential_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int blocksize)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  auto max_label = tier2::maximum_of_all_pixels_func(device, src);
  auto flagged = Array::create(int(max_label + 1), 1, 1, src->dtype(), src->mtype(), src->device());
  flagged->fill(0);
  tier3::flag_existing_labels_func(device, src, flagged);
  tier1::set_column_func(device, flagged, 0, 0);
  auto block_sums =
    Array::create(int((int(max_label) + 1) / blocksize) + 1, 1, 1, src->dtype(), src->mtype(), src->device());
  tier1::sum_reduction_x_func(device, flagged, block_sums, blocksize);
  auto new_indices = Array::create(int(max_label) + 1, 1, 1, src->dtype(), src->mtype(), src->device());
  tier1::block_enumerate_func(device, flagged, block_sums, new_indices, blocksize);
  tier1::replace_intensities_func(device, src, new_indices, dst);
  return dst;
}

auto
threshold_otsu_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  // compute the image histogram
  constexpr int bin = 256;
  const float   min_intensity = tier2::minimum_of_all_pixels_func(device, src);
  const float   max_intensity = tier2::maximum_of_all_pixels_func(device, src);
  auto          hist_array = Array::create(bin, 1, 1, dType::FLOAT, mType::BUFFER, src->device());
  tier3::histogram_func(device, src, hist_array, bin, min_intensity, max_intensity);
  std::vector<float> histogram_array(hist_array->nbElements());
  hist_array->read(histogram_array.data());
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
  const float        nb_pixels = src->nbElements();
  const float        intensity_factor = (max_intensity - min_intensity) / (bin - 1);
  std::vector<float> range(histogram_array.size());
  std::iota(range.begin(), range.end(), 0);
  std::transform(range.begin(), range.end(), range.begin(), [intensity_factor, min_intensity](float intensity) {
    return intensity * intensity_factor + min_intensity;
  });
  sum_1 = std::transform_reduce(
    range.begin(), range.end(), histogram_array.begin(), 0.0f, std::plus<>(), [](float intensity, float hist_value) {
      return intensity * hist_value;
    });
  for (auto index = 0; index < range.size(); ++index)
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
  // Apply threshold and return
  tier0::create_like(src, dst);
  return tier1::greater_constant_func(device, src, dst, threshold);
}


} // namespace cle::tier4
