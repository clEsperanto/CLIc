#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"

#include "utils.hpp"
#include <numeric>

namespace cle::tier4
{

//
// auto
// threshold_otsu_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
// {
//   constexpr int bin = 256;
//   const float   min_intensity = tier2::minimum_of_all_pixels_func(device, src);
//   const float   max_intensity = tier2::maximum_of_all_pixels_func(device, src);
//   auto          hist_array = Array::create(bin, 1, 1, 1, dType::FLOAT, mType::BUFFER, src->device());
//   tier3::histogram_func(device, src, hist_array, bin, min_intensity, max_intensity);
//   std::vector<float> histogram_array(hist_array->size());
//   hist_array->readTo(histogram_array.data());
//   double       threshold = -1;
//   double       max_variance = -1;
//   double       variance = 0;
//   double       sum_1 = 0;
//   double       sum_2 = 0;
//   double       weight_1 = 0;
//   double       weight_2 = 0;
//   double       mean_1 = 0;
//   double       mean_2 = 0;
//   const double nb_pixels = src->size();
//   const double intensity_factor = static_cast<double>(max_intensity - min_intensity);
//   // const double intensity_factor = static_cast<double>(max_intensity - min_intensity) / static_cast<double>(bin -
//   1);
//   // old implementation - to be removed
//   // std::vector<float> range(histogram_array.size());
//   // std::iota(range.begin(), range.end(), 0.0);
//   // std::transform(range.begin(), range.end(), range.begin(), [intensity_factor, min_intensity](float intensity) {
//   //   return intensity * intensity_factor + static_cast<float>(min_intensity);
//   // });
//   //
//   std::vector<double> range(bin);
//   std::iota(range.begin(), range.end(), 0.0f);
//   std::transform(range.begin(), range.end(), range.begin(), [intensity_factor, min_intensity](double value) {
//     return (value * intensity_factor) / (bin - 1) + static_cast<double>(min_intensity);
//   });
//   sum_1 = std::transform_reduce(
//     range.begin(), range.end(), histogram_array.begin(), 0.0f, std::plus<>(), [](double intensity, float hist_value)
//     {
//       return intensity * static_cast<double>(hist_value);
//     });
//   for (size_t index = 0; index < range.size(); ++index)
//   {
//     if (histogram_array[index] == 0)
//     {
//       continue;
//     }
//     weight_1 += histogram_array[index];
//     weight_2 = nb_pixels - weight_1;
//     sum_2 += range[index] * static_cast<double>(histogram_array[index]);
//     mean_1 = sum_2 / weight_1;
//     mean_2 = (sum_1 - sum_2) / weight_2;
//     variance = weight_1 * weight_2 * ((mean_1 - mean_2) * (mean_1 - mean_2));
//     if (variance > max_variance)
//     {
//       threshold = range[index];
//       max_variance = variance;
//     }
//   }
//   std::cout << "Threshold: " << threshold << std::endl;
//   tier0::create_like(src, dst, dType::BINARY);
//   return tier1::greater_constant_func(device, src, dst, static_cast<float>(threshold));
// }


auto
threshold_otsu_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  constexpr int bin = 256;
  const float   min_intensity = tier2::minimum_of_all_pixels_func(device, src);
  const float   max_intensity = tier2::maximum_of_all_pixels_func(device, src);

  auto hist_array = Array::create(bin, 1, 1, 1, dType::FLOAT, mType::BUFFER, src->device());
  tier3::histogram_func(device, src, hist_array, bin, min_intensity, max_intensity);
  std::vector<float> counts(hist_array->size());
  hist_array->readTo(counts.data());


  double range = max_intensity - min_intensity;

  std::vector<double> bin_centers(bin);
  std::iota(bin_centers.begin(), bin_centers.end(), 0.0);
  std::transform(bin_centers.begin(), bin_centers.end(), bin_centers.begin(), [range, min_intensity](double value) {
    return (value * range) / (bin - 1) + static_cast<double>(min_intensity);
  });

  std::vector<double> weight1(bin), weight2(bin), mean1(bin), mean2(bin), variance12(bin - 1);

  // Compute weight1
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

  auto   max_it = std::max_element(variance12.begin(), variance12.end());
  size_t idx = std::distance(variance12.begin(), max_it);

  double threshold = bin_centers[idx];

  std::cout << "Threshold: " << threshold << std::endl;

  tier0::create_like(src, dst, dType::BINARY);
  return tier1::greater_constant_func(device, src, dst, static_cast<float>(threshold));
}


} // namespace cle::tier4
