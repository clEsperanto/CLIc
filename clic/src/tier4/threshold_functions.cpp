#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"

#include "utils.hpp"

#include <cmath>
#include <functional>
#include <limits>
#include <numeric>

namespace cle::tier4
{

namespace
{

constexpr int histogram_bin_count = 256;

// Histogram of an image split into a fixed number of bins, with the corresponding bin center intensities.
struct Histogram
{
  std::vector<double> counts;
  std::vector<double> bin_centers;
  float               min_intensity;
  float               max_intensity;
  double              range;
};

auto
_compute_histogram(const Device::Pointer & device, const Array::Pointer & src, int bin) -> Histogram
{
  const float  min_intensity = tier2::minimum_of_all_pixels_func(device, src);
  const float  max_intensity = tier2::maximum_of_all_pixels_func(device, src);
  const double range = max_intensity - min_intensity;

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

  return Histogram{ std::vector<double>(counts.begin(), counts.end()), std::move(bin_centers), min_intensity, max_intensity, range };
}

// Creates the binary output image and thresholds src at the given intensity.
auto
_binarize(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, double threshold) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::BOOL);
  tier1::greater_constant_func(device, src, dst, static_cast<float>(threshold));
  return dst;
}

} // namespace

auto
threshold_mean_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  const float mean_intensity = tier3::mean_of_all_pixels_func(device, src);
  return _binarize(device, src, dst, mean_intensity);
}

auto
threshold_otsu_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  const int  bin = histogram_bin_count;
  const auto histogram = _compute_histogram(device, src, bin);

  // Compute weight1
  std::vector<double> weight1(bin), weight2(bin), mean1(bin), mean2(bin), variance12(bin - 1);
  std::partial_sum(histogram.counts.begin(), histogram.counts.end(), weight1.begin());

  // Compute weight2
  std::vector<double> reversed_counts(histogram.counts.rbegin(), histogram.counts.rend());
  std::partial_sum(reversed_counts.begin(), reversed_counts.end(), weight2.rbegin());

  // Compute mean1
  std::vector<double> counts_bin_centers(bin);
  std::transform(
    histogram.counts.begin(), histogram.counts.end(), histogram.bin_centers.begin(), counts_bin_centers.begin(), std::multiplies<>());
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
  const auto   max_it = std::max_element(variance12.begin(), variance12.end());
  const size_t idx = std::distance(variance12.begin(), max_it);

  return _binarize(device, src, dst, histogram.bin_centers[idx]);
}

auto
threshold_yen_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  const int  bin = histogram_bin_count;
  const auto histogram = _compute_histogram(device, src, bin);

  // pmf = counts.astype('float32', copy=False) / counts.sum()
  const double total_counts = std::accumulate(histogram.counts.begin(), histogram.counts.end(), 0.0);
  if (total_counts <= 0.0)
  {
    return _binarize(device, src, dst, histogram.min_intensity);
  }
  std::vector<double> pmf(bin);
  std::transform(
    histogram.counts.begin(), histogram.counts.end(), pmf.begin(), [total_counts](double count) { return count / total_counts; });

  // P1 = np.cumsum(pmf)
  std::vector<double> P1(bin);
  std::partial_sum(pmf.begin(), pmf.end(), P1.begin());

  // P1_sq = np.cumsum(pmf**2)
  std::vector<double> pmf_squared(bin);
  std::transform(pmf.begin(), pmf.end(), pmf_squared.begin(), [](double value) { return value * value; });
  std::vector<double> P1_sq(bin);
  std::partial_sum(pmf_squared.begin(), pmf_squared.end(), P1_sq.begin());

  // P2_sq = np.cumsum(pmf[::-1] ** 2)[::-1]
  std::vector<double> reversed_pmf_squared(pmf_squared.rbegin(), pmf_squared.rend());
  std::vector<double> P2_sq(bin);
  std::partial_sum(reversed_pmf_squared.begin(), reversed_pmf_squared.end(), P2_sq.rbegin());

  // crit = np.log(((P1_sq[:-1] * P2_sq[1:]) ** -1) * (P1[:-1] * (1.0 - P1[:-1])) ** 2)
  std::vector<double> crit(bin - 1);
  for (size_t i = 0; i < static_cast<size_t>(bin - 1); ++i)
  {
    double term1 = P1_sq[i] * P2_sq[i + 1];
    double term2 = P1[i] * (1.0 - P1[i]);
    if (term1 > 0.0 && term2 > 0.0)
    {
      double value = (term2 * term2) / term1;
      double logv = std::log(value);
      crit[i] = (std::isfinite(logv)) ? logv : -std::numeric_limits<double>::infinity();
    }
    else
    {
      crit[i] = -std::numeric_limits<double>::infinity();
    }
  }

  // bin_centers[crit.argmax()]
  const auto   max_it = std::max_element(crit.begin(), crit.end());
  const size_t idx = std::distance(crit.begin(), max_it);

  return _binarize(device, src, dst, histogram.bin_centers[idx]);
}

auto
threshold_triangle_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  const int  bin = histogram_bin_count;
  const auto histogram = _compute_histogram(device, src, bin);

  if (histogram.range <= 0.0)
  {
    return _binarize(device, src, dst, histogram.min_intensity);
  }

  // Locate the first and last non-empty histogram bins
  int arg_low_level = -1;
  int arg_high_level = -1;
  for (int i = 0; i < bin; ++i)
  {
    if (histogram.counts[i] > 0)
    {
      if (arg_low_level < 0)
      {
        arg_low_level = i;
      }
      arg_high_level = i;
    }
  }
  if (arg_low_level < 0 || arg_low_level == arg_high_level)
  {
    return _binarize(device, src, dst, histogram.min_intensity);
  }

  // Locate the histogram peak
  int arg_peak_height =
    static_cast<int>(std::distance(histogram.counts.begin(), std::max_element(histogram.counts.begin(), histogram.counts.end())));
  const double peak_height = histogram.counts[arg_peak_height];

  // Flip the histogram if the longer arm is on the left of the peak
  const bool          flip = (arg_peak_height - arg_low_level) < (arg_high_level - arg_peak_height);
  std::vector<double> hist(histogram.counts);
  if (flip)
  {
    std::reverse(hist.begin(), hist.end());
    arg_low_level = bin - arg_high_level - 1;
    arg_peak_height = bin - arg_peak_height - 1;
  }

  // Set up the coordinate system and normalize
  const int    width = arg_peak_height - arg_low_level;
  const double norm = std::sqrt(peak_height * peak_height + static_cast<double>(width) * width);
  const double norm_peak_height = peak_height / norm;
  const double norm_width = width / norm;

  // Maximize the distance between the histogram and the peak-to-tail line
  int    best_x = 0;
  double best_length = -std::numeric_limits<double>::infinity();
  for (int x = 0; x < width; ++x)
  {
    const double y = hist[x + arg_low_level];
    const double length = norm_peak_height * x - norm_width * y;
    if (length > best_length)
    {
      best_length = length;
      best_x = x;
    }
  }
  int arg_level = best_x + arg_low_level;
  if (flip)
  {
    arg_level = bin - arg_level - 1;
  }

  return _binarize(device, src, dst, histogram.bin_centers[arg_level]);
}

auto
threshold_isodata_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  const int  bin = histogram_bin_count;
  const auto histogram = _compute_histogram(device, src, bin);

  if (histogram.range <= 0.0)
  {
    return _binarize(device, src, dst, histogram.min_intensity);
  }

  // csuml[i] / csumh[i]: number of pixels in bin i or lower / strictly higher than bin i
  std::vector<double> csuml(bin);
  std::partial_sum(histogram.counts.begin(), histogram.counts.end(), csuml.begin());
  const double        total = csuml.back();
  std::vector<double> csumh(bin);
  std::transform(csuml.begin(), csuml.end(), csumh.begin(), [total](double value) { return total - value; });

  // Cumulative intensity sum
  std::vector<double> intensity_sum(bin);
  std::transform(
    histogram.counts.begin(), histogram.counts.end(), histogram.bin_centers.begin(), intensity_sum.begin(), std::multiplies<>());
  std::vector<double> csum_intensity(bin);
  std::partial_sum(intensity_sum.begin(), intensity_sum.end(), csum_intensity.begin());

  // Search for the lowest threshold satisfying threshold = (mean_below + mean_above) / 2
  const double bin_width = histogram.bin_centers[1] - histogram.bin_centers[0];
  double       threshold = histogram.bin_centers[bin / 2];
  for (int i = 0; i < bin - 1; ++i)
  {
    if (csuml[i] <= 0.0 || csumh[i] <= 0.0)
    {
      continue;
    }
    const double lower = csum_intensity[i] / csuml[i];
    const double higher = (csum_intensity[bin - 1] - csum_intensity[i]) / csumh[i];
    const double all_mean = (lower + higher) / 2.0;
    const double distance = all_mean - histogram.bin_centers[i];
    if (distance >= 0.0 && distance < bin_width)
    {
      threshold = histogram.bin_centers[i];
      break;
    }
  }

  return _binarize(device, src, dst, threshold);
}

auto
threshold_li_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  const int  bin = histogram_bin_count;
  const auto histogram = _compute_histogram(device, src, bin);

  if (histogram.range <= 0.0)
  {
    return _binarize(device, src, dst, histogram.min_intensity);
  }

  // Shift bin centers so intensities are non-negative, as required by the log(mean) update
  const double        min_intensity = histogram.min_intensity;
  std::vector<double> shifted_centers(bin);
  std::transform(histogram.bin_centers.begin(), histogram.bin_centers.end(), shifted_centers.begin(), [min_intensity](double value) {
    return value - min_intensity;
  });

  // Iterative minimum cross-entropy estimation (Li & Tam, 1998)
  const double tolerance = 0.5 * (histogram.range / (bin - 1));
  const float  mean_intensity = tier3::mean_of_all_pixels_func(device, src);
  double       t_next = static_cast<double>(mean_intensity) - static_cast<double>(histogram.min_intensity);
  double       t_curr = -2.0 * tolerance;

  constexpr int max_iterations = 100;
  int           iteration = 0;
  while (std::abs(t_next - t_curr) > tolerance && iteration < max_iterations)
  {
    t_curr = t_next;

    double sum_fore = 0.0, weight_fore = 0.0;
    double sum_back = 0.0, weight_back = 0.0;
    for (int i = 0; i < bin; ++i)
    {
      if (shifted_centers[i] > t_curr)
      {
        sum_fore += histogram.counts[i] * shifted_centers[i];
        weight_fore += histogram.counts[i];
      }
      else
      {
        sum_back += histogram.counts[i] * shifted_centers[i];
        weight_back += histogram.counts[i];
      }
    }

    if (weight_fore <= 0.0 || weight_back <= 0.0)
    {
      break;
    }

    const double mean_fore = sum_fore / weight_fore;
    const double mean_back = sum_back / weight_back;

    if (mean_back <= 0.0 || mean_fore <= 0.0)
    {
      break;
    }

    t_next = (mean_back - mean_fore) / (std::log(mean_back) - std::log(mean_fore));
    ++iteration;
  }

  return _binarize(device, src, dst, t_next + static_cast<double>(histogram.min_intensity));
}

auto
percentile_func(const Device::Pointer & device, const Array::Pointer & src, const float percentile) -> float
{
  const auto   histogram = _compute_histogram(device, src, histogram_bin_count);
  const auto & frequency = histogram.counts;
  const auto & bin_edges = histogram.bin_centers;

  // compute cumulative sum of the vector frequency
  std::vector<double> cumulative_sum(frequency.size());
  std::partial_sum(frequency.begin(), frequency.end(), cumulative_sum.begin());

  // Calculate total frequency and target frequency
  const double total_frequency = cumulative_sum.back();
  const double target_frequency = total_frequency * (percentile / 100.0);

  // Find the bin containing the target frequency using binary search
  const auto it = std::lower_bound(cumulative_sum.begin(), cumulative_sum.end(), target_frequency);
  const int  index = std::distance(cumulative_sum.begin(), it);

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
