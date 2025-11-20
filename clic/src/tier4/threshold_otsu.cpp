#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"

#include "utils.hpp"

#include <functional>
#include <numeric>

namespace cle::tier4
{

auto
threshold_mean_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  const float mean_intensity = tier3::mean_of_all_pixels_func(device, src);
  tier0::create_like(src, dst, dType::BINARY);
  tier1::greater_constant_func(device, src, dst, mean_intensity);
  return dst;
}

auto
threshold_otsu_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  // Initialize histogram
  constexpr int bin = 256;
  const float   min_intensity = tier2::minimum_of_all_pixels_func(device, src);
  const float   max_intensity = tier2::maximum_of_all_pixels_func(device, src);
  double        range = max_intensity - min_intensity;

  // Compute histogram
  auto hist_array = Array::create(bin, 1, 1, 1, dType::FLOAT, mType::BUFFER, src->device());
  tier3::histogram_func(device, src, hist_array, bin, min_intensity, max_intensity);
  std::vector<float> counts(hist_array->size());
  hist_array->readTo(counts.data());

  // Compute bin centers
  std::vector<double> bin_centers(bin);
  std::iota(bin_centers.begin(), bin_centers.end(), 0.0);
  std::transform(bin_centers.begin(), bin_centers.end(), bin_centers.begin(), [range, min_intensity, bin](double value) {
    return (value * range) / (bin - 1) + static_cast<double>(min_intensity);
  });

  // Compute weight1
  std::vector<double> weight1(bin), weight2(bin), mean1(bin), mean2(bin), variance12(bin - 1);
  std::partial_sum(counts.begin(), counts.end(), weight1.begin());

  // Compute weight2
  std::vector<double> reversed_counts(counts.rbegin(), counts.rend());
  std::partial_sum(reversed_counts.begin(), reversed_counts.end(), weight2.rbegin());

  // Compute mean1
  std::vector<double> counts_bin_centers(bin);
  std::transform(counts.begin(), counts.end(), bin_centers.begin(), counts_bin_centers.begin(), std::multiplies<>());
  std::partial_sum(counts_bin_centers.begin(), counts_bin_centers.end(), mean1.begin());
  std::transform(mean1.begin(), mean1.end(), weight1.begin(), mean1.begin(), std::divides<>());

  // Compute mean2
  std::partial_sum(counts_bin_centers.rbegin(), counts_bin_centers.rend(), mean2.rbegin());
  std::transform(mean2.begin(), mean2.end(), weight2.begin(), mean2.begin(), std::divides<>());

  // Compute variance12
  for (size_t i = 0; i < bin - 1; ++i)
  {
    variance12[i] = weight1[i] * weight2[i + 1] * (mean1[i] - mean2[i + 1]) * (mean1[i] - mean2[i + 1]);
  }

  // Find the maximum variance and threshold value associated with it
  auto   max_it = std::max_element(variance12.begin(), variance12.end());
  size_t idx = std::distance(variance12.begin(), max_it);
  double threshold = bin_centers[idx];

  // Create binary image with threshold
  tier0::create_like(src, dst, dType::BINARY);
  tier1::greater_constant_func(device, src, dst, static_cast<float>(threshold));
  return dst;
}


auto
percentile_func(const Device::Pointer & device, const Array::Pointer & src, const float percentile) -> float
{
  // Initialize histogram
  constexpr int bin = 256;
  const float   min_intensity = tier2::minimum_of_all_pixels_func(device, src);
  const float   max_intensity = tier2::maximum_of_all_pixels_func(device, src);
  double        range = max_intensity - min_intensity;

  // compute bin edges
  std::vector<double> bin_edges(bin);
  std::iota(bin_edges.begin(), bin_edges.end(), 0); // Fill with indices [0, 1, 2, ..., bin-1]
  std::transform(bin_edges.begin(), bin_edges.end(), bin_edges.begin(), [min_intensity, range, bin](double i) {
    return min_intensity + (i * range) / (bin - 1);
  });

  // Compute histogram
  auto hist_array = Array::create(bin, 1, 1, 1, dType::FLOAT, mType::BUFFER, src->device());
  tier3::histogram_func(device, src, hist_array, bin, min_intensity, max_intensity);
  std::vector<float> frequency(hist_array->size());
  hist_array->readTo(frequency.data());

  // compute cumulative sum of the vector frequency
  std::vector<double> cumulative_sum(frequency.size());
  std::partial_sum(frequency.begin(), frequency.end(), cumulative_sum.begin());

  // Calculate total frequency and target frequency
  double total_frequency = cumulative_sum.back();
  double target_frequency = total_frequency * (percentile / 100.0);

  // Find the bin containing the target frequency using binary search
  auto it = std::lower_bound(cumulative_sum.begin(), cumulative_sum.end(), target_frequency);
  int  index = std::distance(cumulative_sum.begin(), it);

  // Compute the percentile value
  const double lower_edge = (index == 0) ? bin_edges[index] : bin_edges[index - 1];
  const double upper_edge = bin_edges[index];
  const double previous_frequency = (index == 0) ? 0.0 : cumulative_sum[index - 1];
  const double fraction = (target_frequency - previous_frequency) / frequency[index];

  float res = static_cast<float>(lower_edge + fraction * (upper_edge - lower_edge));
  if (src->dtype() != dType::FLOAT)
  {
    res = static_cast<float>(std::round(res)); // Round to nearest integer if not float
  }

  return res;
}


} // namespace cle::tier4
