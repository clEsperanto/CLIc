#include "statistics.hpp"

#include "execution.hpp"
#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"
#include "tier6.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <numeric>
namespace
{

// Portable float atomics for OpenCL/CUDA/Metal: no backend natively supports atomic float
// min/max, and only CUDA natively supports atomic float add, so all three are implemented as a
// compare-and-swap loop on the bit-reinterpreted value. Comparisons are done in float domain
// (not on raw bit patterns) to stay correct across negative values. The translators (see
// metalTranslator.cpp::translateBitcast / translateCompareExchange, cudaTranslator.cpp) map
// as_uint/as_float and the "old = atomic_cmpxchg(...);" assignment idiom used below to their
// backend-native equivalents.
constexpr const char * atomic_float_helpers_source = R"CLC(
  inline void atomic_add_float(volatile __global float * addr, float val)
  {
    volatile __global uint * addr_as_uint = (volatile __global uint *) addr;
    uint old = *addr_as_uint;
    uint assumed;
    do {
      assumed = old;
      float current = as_float(assumed);
      uint next = as_uint(current + val);
      old = atomic_cmpxchg(addr_as_uint, assumed, next);
    } while (assumed != old);
  }

  inline void atomic_min_float(volatile __global float * addr, float val)
  {
    volatile __global uint * addr_as_uint = (volatile __global uint *) addr;
    uint old = *addr_as_uint;
    uint assumed;
    do {
      assumed = old;
      float current = as_float(assumed);
      if (current <= val) break;
      uint next = as_uint(val);
      old = atomic_cmpxchg(addr_as_uint, assumed, next);
    } while (assumed != old);
  }

  inline void atomic_max_float(volatile __global float * addr, float val)
  {
    volatile __global uint * addr_as_uint = (volatile __global uint *) addr;
    uint old = *addr_as_uint;
    uint assumed;
    do {
      assumed = old;
      float current = as_float(assumed);
      if (current >= val) break;
      uint next = as_uint(val);
      old = atomic_cmpxchg(addr_as_uint, assumed, next);
    } while (assumed != old);
  }
)CLC";

// The accumulation kernels below do not issue atomics per pixel: a large label (the background when it is included,
// which is most of the image) would make every thread hammer the same few addresses with compare-and-swap loops, which
// serialises the whole launch (seconds on a 1k x 1k image). Instead:
//  - each thread walks a run of `chunk` consecutive pixels along x and accumulates in registers, and only flushes to
//    global memory when the label changes (labels come in runs along x);
//  - the flushes go to one of `nb_slots` copies of the output (chosen per run), merged afterwards by
//    statistics_merge_slots, which divides the contention on a single label by nb_slots.
// Output layout: dst[(slot * nb_planes + plane) * nb_labels + label].
constexpr const char * run_slot_helper_source = R"CLC(
  // Output slot of the run of pixels handled by thread (cx, y, z)
  inline int run_slot(int cx, int y, int z, int width, int height, int chunk, int nb_slots)
  {
    const int nb_chunks = (width + chunk - 1) / chunk;
    return ((z * height + y) * nb_chunks + cx) % nb_slots;
  }
)CLC";

constexpr const char * statistics_accumulate_kernel_body = R"CLC(
  __constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

  __kernel void statistics_accumulate(
      IMAGE_src_label_TYPE src_label,
      IMAGE_src_image_TYPE src_image,
      IMAGE_dst_TYPE        dst,
      int                   nb_labels,
      int                   chunk,
      int                   nb_slots
  )
  {
    const int cx = get_global_id(0);
    const int y = get_global_id(1);
    const int z = get_global_id(2);

    const int w = GET_IMAGE_WIDTH(src_label);
    const int h = GET_IMAGE_HEIGHT(src_label);
    const int x0 = cx * chunk;
    // CUDA launches whole blocks: threads beyond the image would accumulate the clamped edge pixels
    if (x0 >= w || y >= h || z >= GET_IMAGE_DEPTH(src_label))
      return;
    const int x1 = min(x0 + chunk, w);
    const int slot_offset = run_slot(cx, y, z, w, h, chunk, nb_slots) * 16 * nb_labels;

    // a run covers [start, x - 1] at a single (y, z): only the intensity terms need to be accumulated
    int   current = 0;
    int   start = x0;
    float count = 0, sum_i = 0, sum_ix = 0, min_i = 0, max_i = 0;
    for (int x = x0; x <= x1; ++x)
    {
      // x == x1 is a sentinel that flushes the last run
      const int label = (x < x1) ? (int) READ_IMAGE(src_label, sampler, POS_src_label_INSTANCE(x, y, z, 0)).x : -1;
      if (label != current)
      {
        if (current > 0)
        {
          const int base = slot_offset + current;
          atomic_add_float(&dst[base + 0 * nb_labels], (float) (start + x - 1) * count / 2);
          atomic_add_float(&dst[base + 1 * nb_labels], (float) y * count);
          atomic_add_float(&dst[base + 2 * nb_labels], (float) z * count);
          atomic_add_float(&dst[base + 3 * nb_labels], count);
          atomic_add_float(&dst[base + 4 * nb_labels], sum_ix);
          atomic_add_float(&dst[base + 5 * nb_labels], (float) y * sum_i);
          atomic_add_float(&dst[base + 6 * nb_labels], (float) z * sum_i);
          atomic_add_float(&dst[base + 7 * nb_labels], sum_i);
          atomic_min_float(&dst[base + 8 * nb_labels], min_i);
          atomic_max_float(&dst[base + 9 * nb_labels], max_i);
          atomic_min_float(&dst[base + 10 * nb_labels], (float) start);
          atomic_max_float(&dst[base + 11 * nb_labels], (float) (x - 1));
          atomic_min_float(&dst[base + 12 * nb_labels], (float) y);
          atomic_max_float(&dst[base + 13 * nb_labels], (float) y);
          atomic_min_float(&dst[base + 14 * nb_labels], (float) z);
          atomic_max_float(&dst[base + 15 * nb_labels], (float) z);
        }
        current = label;
        start = x;
        count = 0;
        sum_i = 0;
        sum_ix = 0;
      }
      if (current <= 0)
        continue;

      const float value = (float) READ_IMAGE(src_image, sampler, POS_src_image_INSTANCE(x, y, z, 0)).x;
      min_i = (count == 0) ? value : min(min_i, value);
      max_i = (count == 0) ? value : max(max_i, value);
      count += 1.0f;
      sum_i += value;
      sum_ix += (float) x * value;
    }
  }
)CLC";

constexpr const char * std_accumulate_kernel_body = R"CLC(
  __constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

  __kernel void std_accumulate(
      IMAGE_src_statistics_TYPE src_statistics,
      IMAGE_src_label_TYPE      src_label,
      IMAGE_src_image_TYPE      src_image,
      IMAGE_dst_TYPE            dst,
      int                       nb_labels,
      int                       chunk,
      int                       nb_slots
  )
  {
    const int cx = get_global_id(0);
    const int y = get_global_id(1);
    const int z = get_global_id(2);

    const int w = GET_IMAGE_WIDTH(src_label);
    const int h = GET_IMAGE_HEIGHT(src_label);
    const int x0 = cx * chunk;
    // CUDA launches whole blocks: threads beyond the image would accumulate the clamped edge pixels
    if (x0 >= w || y >= h || z >= GET_IMAGE_DEPTH(src_label))
      return;
    const int x1 = min(x0 + chunk, w);
    const int slot_offset = run_slot(cx, y, z, w, h, chunk, nb_slots) * 6 * nb_labels;

    int   current = 0;
    float centroid_x = 0, centroid_y = 0, centroid_z = 0;
    float mass_center_x = 0, mass_center_y = 0, mass_center_z = 0;
    float mean_intensity = 0, area = 1;
    float sum_cd = 0, sum_md = 0, sum_diff = 0, count = 0, max_cd = 0, max_md = 0;
    for (int x = x0; x <= x1; ++x)
    {
      // x == x1 is a sentinel that flushes the last run
      const int label = (x < x1) ? (int) READ_IMAGE(src_label, sampler, POS_src_label_INSTANCE(x, y, z, 0)).x : -1;
      if (label != current)
      {
        if (current > 0)
        {
          const int base = slot_offset + current;
          atomic_add_float(&dst[base + 0 * nb_labels], sum_cd);
          atomic_add_float(&dst[base + 1 * nb_labels], sum_md);
          atomic_add_float(&dst[base + 2 * nb_labels], sum_diff);
          atomic_add_float(&dst[base + 3 * nb_labels], count);
          atomic_max_float(&dst[base + 4 * nb_labels], max_cd);
          atomic_max_float(&dst[base + 5 * nb_labels], max_md);
        }
        current = label;
        sum_cd = 0; sum_md = 0; sum_diff = 0; count = 0; max_cd = 0; max_md = 0;
        if (current > 0)
        {
          centroid_x = READ_IMAGE(src_statistics, sampler, POS_src_statistics_INSTANCE(current, 0, 0, 0)).x;
          centroid_y = READ_IMAGE(src_statistics, sampler, POS_src_statistics_INSTANCE(current, 1, 0, 0)).x;
          centroid_z = READ_IMAGE(src_statistics, sampler, POS_src_statistics_INSTANCE(current, 2, 0, 0)).x;
          mass_center_x = READ_IMAGE(src_statistics, sampler, POS_src_statistics_INSTANCE(current, 3, 0, 0)).x;
          mass_center_y = READ_IMAGE(src_statistics, sampler, POS_src_statistics_INSTANCE(current, 4, 0, 0)).x;
          mass_center_z = READ_IMAGE(src_statistics, sampler, POS_src_statistics_INSTANCE(current, 5, 0, 0)).x;
          mean_intensity = READ_IMAGE(src_statistics, sampler, POS_src_statistics_INSTANCE(current, 6, 0, 0)).x;
          area = READ_IMAGE(src_statistics, sampler, POS_src_statistics_INSTANCE(current, 7, 0, 0)).x;
        }
      }
      if (current <= 0)
        continue;

      const float value = (float) READ_IMAGE(src_image, sampler, POS_src_image_INSTANCE(x, y, z, 0)).x;
      const float centroid_distance = sqrt(
        pow((float)x - centroid_x, (float)2.0) +
        pow((float)y - centroid_y, (float)2.0) +
        pow((float)z - centroid_z, (float)2.0)
      );
      const float mass_center_distance = sqrt(
        pow((float)x - mass_center_x, (float)2.0) +
        pow((float)y - mass_center_y, (float)2.0) +
        pow((float)z - mass_center_z, (float)2.0)
      );
      sum_cd += centroid_distance;
      sum_md += mass_center_distance;
      sum_diff += pow(value - mean_intensity, (float)2.0) / area;
      count += 1.0f;
      max_cd = max(max_cd, centroid_distance);
      max_md = max(max_md, mass_center_distance);
    }
  }
)CLC";

// Planes [0, nb_sums) hold sums, the following ones extrema: alternately minimum and maximum (min_max_pairs = 1),
// or only maxima (min_max_pairs = 0). The layout arguments come last, in the same order, in both kernels.
constexpr const char * plane_kind_helper_source = R"CLC(
  inline int is_minimum_plane(int plane, int nb_sums, int min_max_pairs)
  {
    return plane >= nb_sums && min_max_pairs && (plane - nb_sums) % 2 == 0;
  }
)CLC";

constexpr const char * statistics_init_slots_kernel_body = R"CLC(
  __kernel void statistics_init_slots(
      IMAGE_dst_TYPE dst,
      float          min_init,
      float          max_init,
      int            nb_labels,
      int            nb_planes,
      int            nb_sums,
      int            min_max_pairs
  )
  {
    const int label = get_global_id(0);
    const int row = get_global_id(1);
    if (label >= nb_labels || row >= GET_IMAGE_HEIGHT(dst))
      return;
    const int plane = row % nb_planes;
    float value = 0;
    if (plane >= nb_sums)
      value = is_minimum_plane(plane, nb_sums, min_max_pairs) ? min_init : max_init;
    dst[row * nb_labels + label] = value;
  }
)CLC";

constexpr const char * statistics_merge_slots_kernel_body = R"CLC(
  __kernel void statistics_merge_slots(
      IMAGE_src_TYPE src,
      IMAGE_dst_TYPE dst,
      int            nb_slots,
      int            nb_labels,
      int            nb_planes,
      int            nb_sums,
      int            min_max_pairs
  )
  {
    const int label = get_global_id(0);
    const int plane = get_global_id(1);
    if (label >= nb_labels || plane >= nb_planes)
      return;
    const int is_minimum = is_minimum_plane(plane, nb_sums, min_max_pairs);
    float     acc = src[plane * nb_labels + label];
    for (int s = 1; s < nb_slots; ++s)
    {
      const float v = src[(s * nb_planes + plane) * nb_labels + label];
      if (plane < nb_sums)
        acc += v;
      else if (is_minimum)
        acc = min(acc, v);
      else
        acc = max(acc, v);
    }
    dst[plane * nb_labels + label] = acc;
  }
)CLC";

// Pixels per thread along x, and memory budget (in floats) for the output slots
constexpr int    statistics_chunk = 64;
constexpr size_t statistics_slot_budget = size_t(1) << 22;

} // namespace

namespace cle
{

// Run an accumulation kernel (statistics_accumulate / std_accumulate) through the slotted buffer and merge the slots.
// `params` holds the kernel inputs; dst, nb_labels, chunk and nb_slots are appended here. Maxima start at max_init.
static auto
_accumulate_per_label(const Device::Pointer & device,
                      const std::string &     name,
                      const char *            body,
                      ParameterList           params,
                      const Array::Pointer &  label,
                      int                     nb_labels,
                      int                     nb_planes,
                      int                     nb_sums,
                      int                     min_max_pairs,
                      float                   max_init) -> Array::Pointer
{
  const size_t per_slot = static_cast<size_t>(nb_planes) * static_cast<size_t>(nb_labels);
  const int    nb_slots = static_cast<int>(std::clamp<size_t>(statistics_slot_budget / per_slot, 1, 64));

  const ParameterList layout = {
    { "nb_labels", nb_labels }, { "nb_planes", nb_planes }, { "nb_sums", nb_sums }, { "min_max_pairs", min_max_pairs }
  };
  const auto with_layout = [&layout](ParameterList list) {
    list.insert(list.end(), layout.begin(), layout.end());
    return list;
  };

  auto slots = Array::create(nb_labels, nb_planes * nb_slots, 1, 2, dType::FLOAT, mType::BUFFER, device);
  execute(device,
          { "statistics_init_slots", std::string(plane_kind_helper_source) + statistics_init_slots_kernel_body },
          with_layout({ { "dst", slots }, { "min_init", std::numeric_limits<float>::max() }, { "max_init", max_init } }),
          { slots->width(), slots->height(), 1 });

  params.insert(params.end(), { { "dst", slots }, { "nb_labels", nb_labels }, { "chunk", statistics_chunk }, { "nb_slots", nb_slots } });
  execute(device,
          { name, std::string(atomic_float_helpers_source) + run_slot_helper_source + body },
          params,
          { (label->width() + statistics_chunk - 1) / statistics_chunk, label->height(), label->depth() });

  auto result = Array::create(nb_labels, nb_planes, 1, 2, dType::FLOAT, mType::BUFFER, device);
  execute(device,
          { "statistics_merge_slots", std::string(plane_kind_helper_source) + statistics_merge_slots_kernel_body },
          with_layout({ { "src", slots }, { "dst", result }, { "nb_slots", nb_slots } }),
          { result->width(), result->height(), 1 });
  return result;
}

auto
_statistics_per_label(const Device::Pointer & device, const Array::Pointer & label, const Array::Pointer & intensity, int nb_labels)
  -> Array::Pointer
{
  // 16 planes per label (see statistics_accumulate above): 8 sums, then 4 min/max pairs
  return _accumulate_per_label(device,
                               "statistics_accumulate",
                               statistics_accumulate_kernel_body,
                               { { "src_label", label }, { "src_image", intensity } },
                               label,
                               nb_labels,
                               16,
                               8,
                               1,
                               std::numeric_limits<float>::lowest());
}

auto
_std_per_label(const Device::Pointer & device,
               const Array::Pointer &  statistics,
               const Array::Pointer &  label,
               const Array::Pointer &  intensity,
               int                     nb_labels) -> Array::Pointer
{
  // 6 planes per label (see std_accumulate above): 4 sums, then 2 maxima of distances (start at 0, the value of a
  // label without pixels); must run strictly after `statistics` (centroid/mass-center/mean-intensity/area) is final.
  return _accumulate_per_label(device,
                               "std_accumulate",
                               std_accumulate_kernel_body,
                               { { "src_statistics", statistics }, { "src_label", label }, { "src_image", intensity } },
                               label,
                               nb_labels,
                               6,
                               4,
                               0,
                               0.0F);
}

auto
compute_statistics_per_labels(const Device::Pointer & device, const Array::Pointer & label, const Array::Pointer & intensity)
  -> StatisticsMap
{
  // The GPU only accumulates (two passes, see statistics_accumulate and std_accumulate); each pass is read back once and
  // every property is derived on the host, instead of one copy, kernel launch and read-back per property.
  const size_t offset = 1; // label 0 (background) is not reported
  const size_t nb_labels = static_cast<size_t>(tier1::maximum_of_all_pixels_func(device, label)) + offset;
  const size_t n = nb_labels - offset;

  // stats[p * nb_labels + l]: plane p of label l (16 planes, see statistics_accumulate)
  const auto         stats_per_label = _statistics_per_label(device, label, intensity, static_cast<int>(nb_labels));
  std::vector<float> stats(16 * nb_labels);
  stats_per_label->readTo(stats.data());
  const auto plane = [&](const std::vector<float> & planes, size_t p) {
    const auto first = planes.begin() + static_cast<std::ptrdiff_t>(p * nb_labels + offset);
    return std::vector<float>(first, first + static_cast<std::ptrdiff_t>(n));
  };
  const auto divide = [n](const std::vector<float> & a, const std::vector<float> & b) {
    std::vector<float> r(n);
    std::transform(a.begin(), a.end(), b.begin(), r.begin(), std::divides<float>());
    return r;
  };

  cle::StatisticsMap region_props;
  region_props.reserve(37);

  std::vector<float> labels_list(n);
  std::iota(labels_list.begin(), labels_list.end(), static_cast<float>(offset));
  region_props["label"] = std::move(labels_list);

  // bounding box (planes 10 to 15: min/max x, y, z)
  const std::vector<std::string> dim_names = { "x", "y", "z" };
  const std::vector<std::string> extent_names = { "width", "height", "depth" };
  for (size_t dim = 0; dim < 3; ++dim)
  {
    auto               bbox_min = plane(stats, 10 + 2 * dim);
    auto               bbox_max = plane(stats, 11 + 2 * dim);
    std::vector<float> extent(n);
    for (size_t i = 0; i < n; ++i)
    {
      extent[i] = bbox_max[i] - bbox_min[i] + 1;
    }
    region_props["bbox_min_" + dim_names[dim]] = std::move(bbox_min);
    region_props["bbox_max_" + dim_names[dim]] = std::move(bbox_max);
    region_props["bbox_" + extent_names[dim]] = std::move(extent);
  }

  // area (plane 3), intensity (planes 7 to 9), mass center (planes 4 to 6) and centroid (planes 0 to 2)
  const auto area = plane(stats, 3);
  const auto sum_intensity = plane(stats, 7);
  region_props["area"] = area;
  region_props["min_intensity"] = plane(stats, 8);
  region_props["max_intensity"] = plane(stats, 9);
  region_props["sum_intensity"] = sum_intensity;
  region_props["mean_intensity"] = divide(sum_intensity, area);
  for (size_t dim = 0; dim < 3; ++dim)
  {
    const auto sum_intensity_times = plane(stats, 4 + dim);
    region_props["mass_center_" + dim_names[dim]] = divide(sum_intensity_times, sum_intensity);
    region_props["sum_intensity_times_" + dim_names[dim]] = sum_intensity_times;
    const auto sum = plane(stats, dim);
    region_props["centroid_" + dim_names[dim]] = divide(sum, area);
    region_props["sum_" + dim_names[dim]] = sum;
  }

  // Second part: parameters which depend on the first ones. std_accumulate reads, per label, 8 planes: centroid x/y/z,
  // mass center x/y/z, mean intensity, area (label 0 is never read).
  const std::vector<std::string> reference_planes = { "centroid_x",    "centroid_y",    "centroid_z",     "mass_center_x",
                                                      "mass_center_y", "mass_center_z", "mean_intensity", "area" };
  std::vector<float>             reference(reference_planes.size() * nb_labels, 0);
  for (size_t p = 0; p < reference_planes.size(); ++p)
  {
    const auto & values = region_props[reference_planes[p]];
    std::copy(values.begin(), values.end(), reference.begin() + static_cast<std::ptrdiff_t>(p * nb_labels + offset));
  }
  auto label_statistics_image = Array::create(nb_labels, reference_planes.size(), 1, 2, dType::FLOAT, mType::BUFFER, device);
  label_statistics_image->writeFrom(reference.data());

  // std[p * nb_labels + l] (6 planes, see std_accumulate)
  const auto         std_per_label = _std_per_label(device, label_statistics_image, label, intensity, static_cast<int>(nb_labels));
  std::vector<float> std_stats(6 * nb_labels);
  std_per_label->readTo(std_stats.data());

  const auto         sum_distance_to_centroid = plane(std_stats, 0);
  const auto         sum_distance_to_mass_center = plane(std_stats, 1);
  const auto         mean_distance_to_centroid = divide(sum_distance_to_centroid, area);
  const auto         mean_distance_to_mass_center = divide(sum_distance_to_mass_center, area);
  const auto         max_distance_to_centroid = plane(std_stats, 4);
  const auto         max_distance_to_mass_center = plane(std_stats, 5);
  std::vector<float> standard_deviation_intensity = plane(std_stats, 2); // sum of squared differences / area
  std::transform(standard_deviation_intensity.begin(),
                 standard_deviation_intensity.end(),
                 standard_deviation_intensity.begin(),
                 [](float v) { return std::pow(v, 0.5F); });

  region_props["sum_distance_to_centroid"] = sum_distance_to_centroid;
  region_props["mean_distance_to_centroid"] = mean_distance_to_centroid;
  region_props["sum_distance_to_mass_center"] = sum_distance_to_mass_center;
  region_props["mean_distance_to_mass_center"] = mean_distance_to_mass_center;
  region_props["standard_deviation_intensity"] = std::move(standard_deviation_intensity);
  region_props["max_distance_to_centroid"] = max_distance_to_centroid;
  region_props["max_distance_to_mass_center"] = max_distance_to_mass_center;
  region_props["mean_max_distance_to_centroid_ratio"] = divide(max_distance_to_centroid, mean_distance_to_centroid);
  region_props["mean_max_distance_to_mass_center_ratio"] = divide(max_distance_to_mass_center, mean_distance_to_mass_center);

  return region_props;
}


auto
compute_neighbors_statistics_per_labels(const Device::Pointer &  device,
                                        const Array::Pointer &   label,
                                        const std::vector<int> & proximal_distances,
                                        const std::vector<int> & nearest_neighbor_ns,
                                        const std::vector<int> & dilation_radii) -> StatisticsMap
{
  const std::array<float, 6> touch_ratio_thresholds = { 0, 0.16, 0.2, 0.33, 0.5, 0.75 };

  const auto nb_labels = static_cast<size_t>(tier1::maximum_of_all_pixels_func(device, label)) + 1;
  const auto nb_proximal_distances = proximal_distances.size();
  const auto nb_nearest_neighbor_ns = nearest_neighbor_ns.size();
  const auto nb_dilation_radii = dilation_radii.size();
  const auto nb_touch_ratio_thresholds = touch_ratio_thresholds.size();

  const auto         nb_stats = nb_proximal_distances + 2 * nb_nearest_neighbor_ns + 6 * nb_dilation_radii + nb_touch_ratio_thresholds + 13;
  cle::StatisticsMap nei_stats;
  nei_stats.reserve(nb_stats);

  auto result_device_vector = Array::create(nb_labels, 1, 1, 1, dType::FLOAT, mType::BUFFER, device);

  auto centroids = tier4::centroids_of_labels_func(device, label, nullptr, false);
  auto distance_matrix = tier1::generate_distance_matrix_func(device, centroids, centroids, nullptr);
  auto touching_matrix = tier3::generate_touch_matrix_func(device, label, nullptr);
  tier1::set_column_func(device, touching_matrix, 0, 0);
  tier1::set_row_func(device, touching_matrix, 0, 0);

  // labels include background #0 during computation
  std::vector<float> labels_list(nb_labels);
  std::iota(labels_list.begin(), labels_list.end(), 0.0F);
  nei_stats["label"] = std::move(labels_list);

  // touching neighbor count # 1
  tier2::count_touching_neighbors_func(device, touching_matrix, result_device_vector, true);
  auto & touching_neighbor_count = nei_stats["touching_neighbor_count"];
  touching_neighbor_count.resize(nb_labels);
  result_device_vector->readTo(touching_neighbor_count.data());

  // distances of touching neighbors # 2-4
  tier1::minimum_distance_touching_neighbors_func(device, distance_matrix, touching_matrix, result_device_vector);
  auto & minimum_distance_of_touching_neighbors = nei_stats["minimum_distance_of_touching_neighbors"];
  minimum_distance_of_touching_neighbors.resize(nb_labels);
  result_device_vector->readTo(minimum_distance_of_touching_neighbors.data());
  tier1::average_distance_touching_neighbors_func(device, distance_matrix, touching_matrix, result_device_vector);
  auto & average_distance_of_touching_neighbors = nei_stats["average_distance_of_touching_neighbors"];
  average_distance_of_touching_neighbors.resize(nb_labels);
  result_device_vector->readTo(average_distance_of_touching_neighbors.data());
  tier1::maximum_distance_touching_neighbors_func(device, distance_matrix, touching_matrix, result_device_vector);
  auto & maximum_distance_of_touching_neighbors = nei_stats["maximum_distance_of_touching_neighbors"];
  maximum_distance_of_touching_neighbors.resize(nb_labels);
  result_device_vector->readTo(maximum_distance_of_touching_neighbors.data());

  // max/min distance ratio of touching neighbors # 5
  auto & max_min_distance_ratio_of_touching_neighbors = nei_stats["max_min_distance_ratio_of_touching_neighbors"];
  max_min_distance_ratio_of_touching_neighbors.resize(nb_labels);
  std::transform(nei_stats["maximum_distance_of_touching_neighbors"].begin(),
                 nei_stats["maximum_distance_of_touching_neighbors"].end(),
                 nei_stats["minimum_distance_of_touching_neighbors"].begin(),
                 max_min_distance_ratio_of_touching_neighbors.begin(),
                 [](float max_dist, float min_dist) { return max_dist / min_dist; });

  // number of neighbors within given radii # 6-...
  for (auto d : proximal_distances)
  {
    auto proximal_touch_matrix = tier2::generate_proximal_neighbors_matrix_func(device, distance_matrix, nullptr, 0, d);
    tier2::count_touching_neighbors_func(device, proximal_touch_matrix, result_device_vector, true);
    auto & proximal_neighbor_count_d = nei_stats["proximal_neighbor_count_d" + std::to_string(d)];
    proximal_neighbor_count_d.resize(nb_labels);
    result_device_vector->readTo(proximal_neighbor_count_d.data());
  }

  auto self_far_distance_matrix = tier1::copy_func(device, distance_matrix, nullptr);
  auto max_distance = tier1::maximum_of_all_pixels_func(device, distance_matrix);
  tier1::set_where_x_equals_y_func(device, self_far_distance_matrix, max_distance + 1);

  // maximum and average distance of n nearest neighbors # ... and distance of most distant other neighbor
  for (auto n : nearest_neighbor_ns)
  {
    auto max_distance_of_n_shortest_distances =
      tier1::maximum_distance_n_nearest_neighbors_func(device, self_far_distance_matrix, nullptr, n);
    auto & maximum_distance_of_n_nearest_neighbors = nei_stats["maximum_distance_of_n" + std::to_string(n) + "_nearest_neighbors"];
    maximum_distance_of_n_nearest_neighbors.resize(nb_labels);
    max_distance_of_n_shortest_distances->readTo(maximum_distance_of_n_nearest_neighbors.data());

    auto average_distance_of_n_nearest_distances =
      tier1::mean_distance_n_nearest_neighbors_func(device, self_far_distance_matrix, nullptr, n);
    auto & average_distance_of_n_nearest_neighbors = nei_stats["average_distance_of_n" + std::to_string(n) + "_nearest_neighbors"];
    average_distance_of_n_nearest_neighbors.resize(nb_labels);
    average_distance_of_n_nearest_distances->readTo(average_distance_of_n_nearest_neighbors.data());
  }

  // distance to most distant other neighbor
  auto & distance_to_most_distant_other = nei_stats["distance_to_most_distant_other"];
  distance_to_most_distant_other.resize(nb_labels);
  tier1::mean_distance_n_farthest_neighbors_func(device, self_far_distance_matrix, result_device_vector, 1);
  result_device_vector->readTo(distance_to_most_distant_other.data());

  // touch count / portion
  auto touch_count_matrix = tier3::generate_touching_area_matrix_func(device, label, nullptr);
  auto touch_portion_matrix = tier4::generate_partial_touching_area_matrix_func(device, label, nullptr);
  tier1::set_column_func(device, touch_count_matrix, 0, 0);
  tier1::set_row_func(device, touch_count_matrix, 0, 0);
  tier1::set_column_func(device, touch_portion_matrix, 0, 0);
  tier1::set_row_func(device, touch_portion_matrix, 0, 0);

  // set all 0 pixels to a high value to detect the minimum touch count/portion
  auto max_touch_count = tier1::maximum_of_all_pixels_func(device, touch_count_matrix);
  auto touch_count_matrix_0_set_to_high = tier1::replace_intensity_func(device, touch_count_matrix, nullptr, 0, max_touch_count);
  for (auto t : touch_ratio_thresholds)
  {
    auto touch_portion_within_range_neighbors_matrix =
      tier2::generate_partial_touching_area_matrix_within_range_func(device, touch_portion_matrix, nullptr, t, 1.1f);
    tier2::count_touching_neighbors_func(device, touch_portion_within_range_neighbors_matrix, result_device_vector, true);
    auto & touch_portion_above_threshold_neighbor_count = nei_stats["touch_portion_above_" + std::to_string(t) + "_neighbor_count"];
    touch_portion_above_threshold_neighbor_count.resize(nb_labels);
    result_device_vector->readTo(touch_portion_above_threshold_neighbor_count.data());
  }

  auto temp = tier1::nan_to_num_func(device, touch_portion_matrix, nullptr);
  auto max_touch_portion = tier1::maximum_of_all_pixels_func(device, temp);
  auto touch_portion_matrix_0_set_to_high = tier1::replace_intensity_func(device, temp, nullptr, 0, max_touch_portion);

  auto & touch_count_sum = nei_stats["touch_count_sum"];
  touch_count_sum.resize(nb_labels);
  tier1::sum_y_projection_func(device, touch_count_matrix, result_device_vector);
  result_device_vector->readTo(touch_count_sum.data());
  auto & minimum_touch_count = nei_stats["minimum_touch_count"];
  minimum_touch_count.resize(nb_labels);
  tier1::minimum_y_projection_func(device, touch_count_matrix_0_set_to_high, result_device_vector);
  result_device_vector->readTo(minimum_touch_count.data());
  auto & maximum_touch_count = nei_stats["maximum_touch_count"];
  maximum_touch_count.resize(nb_labels);
  tier1::maximum_y_projection_func(device, touch_count_matrix, result_device_vector);
  result_device_vector->readTo(maximum_touch_count.data());
  auto & minimum_touch_portion = nei_stats["minimum_touch_portion"];
  minimum_touch_portion.resize(nb_labels);
  tier1::minimum_y_projection_func(device, touch_portion_matrix_0_set_to_high, result_device_vector);
  result_device_vector->readTo(minimum_touch_portion.data());
  auto & maximum_touch_portion = nei_stats["maximum_touch_portion"];
  maximum_touch_portion.resize(nb_labels);
  tier1::maximum_y_projection_func(device, touch_portion_matrix, result_device_vector);
  result_device_vector->readTo(maximum_touch_portion.data());

  auto & standard_deviation_touch_portion = nei_stats["standard_deviation_touch_portion"];
  standard_deviation_touch_portion.resize(nb_labels);
  tier5::standard_deviation_partial_touching_area_matrix_func(device, label, result_device_vector);
  result_device_vector->readTo(standard_deviation_touch_portion.data());

  // dilate label image and analye touching neighbors

  for (auto r : dilation_radii)
  {
    auto dilated_label_image = tier6::dilate_labels_func(device, label, nullptr, r);
    auto dilated_centroids = tier4::centroids_of_labels_func(device, dilated_label_image, nullptr, false);
    auto dilated_distance_matrix = tier1::generate_distance_matrix_func(device, dilated_centroids, dilated_centroids, nullptr);
    auto dilated_touch_matrix = tier3::generate_touch_matrix_func(device, dilated_label_image, nullptr);
    tier1::set_column_func(device, dilated_touch_matrix, 0, 0);
    tier1::set_row_func(device, dilated_touch_matrix, 0, 0);

    auto & touching_neighbor_count_dilated_r = nei_stats["touching_neighbor_count_dilated_r_" + std::to_string(r)];
    touching_neighbor_count_dilated_r.resize(nb_labels);
    tier2::count_touching_neighbors_func(device, dilated_touch_matrix, result_device_vector, true);
    result_device_vector->readTo(touching_neighbor_count_dilated_r.data());

    auto & minimum_distance_of_touching_neighbors_dilated_r =
      nei_stats["minimum_distance_of_touching_neighbors_dilated_r_" + std::to_string(r)];
    minimum_distance_of_touching_neighbors_dilated_r.resize(nb_labels);
    tier1::minimum_distance_touching_neighbors_func(device, dilated_distance_matrix, dilated_touch_matrix, result_device_vector);
    result_device_vector->readTo(minimum_distance_of_touching_neighbors_dilated_r.data());
    auto & average_distance_of_touching_neighbors_dilated_r =
      nei_stats["average_distance_of_touching_neighbors_dilated_r_" + std::to_string(r)];
    average_distance_of_touching_neighbors_dilated_r.resize(nb_labels);
    tier1::mean_distance_touching_neighbors_func(device, dilated_distance_matrix, dilated_touch_matrix, result_device_vector);
    result_device_vector->readTo(average_distance_of_touching_neighbors_dilated_r.data());
    auto & maximum_distance_of_touching_neighbors_dilated_r =
      nei_stats["maximum_distance_of_touching_neighbors_dilated_r_" + std::to_string(r)];
    maximum_distance_of_touching_neighbors_dilated_r.resize(nb_labels);
    tier1::maximum_distance_touching_neighbors_func(device, dilated_distance_matrix, dilated_touch_matrix, result_device_vector);
    result_device_vector->readTo(maximum_distance_of_touching_neighbors_dilated_r.data());


    auto & max_min_distance_ratio_of_touching_neighbors_dilated_r =
      nei_stats["max_min_distance_ratio_of_touching_neighbors_dilated_r_" + std::to_string(r)];
    max_min_distance_ratio_of_touching_neighbors_dilated_r.resize(nb_labels);
    std::transform(nei_stats["maximum_distance_of_touching_neighbors_dilated_r_" + std::to_string(r)].begin(),
                   nei_stats["maximum_distance_of_touching_neighbors_dilated_r_" + std::to_string(r)].end(),
                   nei_stats["minimum_distance_of_touching_neighbors_dilated_r_" + std::to_string(r)].begin(),
                   max_min_distance_ratio_of_touching_neighbors_dilated_r.begin(),
                   [](float max_dist, float min_dist) { return max_dist / min_dist; });

    auto dilated_touch_count_matrix = tier3::generate_touching_area_matrix_func(device, dilated_label_image, nullptr);
    auto dilated_touch_portion_matrix = tier4::generate_partial_touching_area_matrix_func(device, dilated_label_image, nullptr);
    tier1::set_column_func(device, dilated_touch_count_matrix, 0, 0);
    tier1::set_row_func(device, dilated_touch_count_matrix, 0, 0);
    tier1::set_column_func(device, dilated_touch_portion_matrix, 0, 0);
    tier1::set_row_func(device, dilated_touch_portion_matrix, 0, 0);

    auto max_dilated_touch_count = tier1::maximum_of_all_pixels_func(device, dilated_touch_count_matrix);
    auto dilated_touch_count_matrix_0_set_to_high =
      tier1::replace_intensity_func(device, dilated_touch_count_matrix, nullptr, 0, max_dilated_touch_count);
    auto temp = tier1::nan_to_num_func(device, dilated_touch_portion_matrix, nullptr);
    auto max_dilated_touch_portion = tier1::maximum_of_all_pixels_func(device, temp);
    auto dilated_touch_portion_matrix_0_set_to_high = tier1::replace_intensity_func(device, temp, nullptr, 0, max_dilated_touch_portion);

    auto & touch_count_sum_dilated_r = nei_stats["touch_count_sum_dilated_r_" + std::to_string(r)];
    touch_count_sum_dilated_r.resize(nb_labels);
    tier1::sum_y_projection_func(device, dilated_touch_count_matrix, result_device_vector);
    result_device_vector->readTo(touch_count_sum_dilated_r.data());
    auto & minimum_touch_count_dilated_r = nei_stats["minimum_touch_count_dilated_r_" + std::to_string(r)];
    minimum_touch_count_dilated_r.resize(nb_labels);
    tier1::minimum_y_projection_func(device, dilated_touch_count_matrix_0_set_to_high, result_device_vector);
    result_device_vector->readTo(minimum_touch_count_dilated_r.data());
    auto & maximum_touch_count_dilated_r = nei_stats["maximum_touch_count_dilated_r_" + std::to_string(r)];
    maximum_touch_count_dilated_r.resize(nb_labels);
    tier1::maximum_y_projection_func(device, dilated_touch_count_matrix, result_device_vector);
    result_device_vector->readTo(maximum_touch_count_dilated_r.data());
    auto & minimum_touch_portion_dilated_r = nei_stats["minimum_touch_portion_dilated_r_" + std::to_string(r)];
    minimum_touch_portion_dilated_r.resize(nb_labels);
    tier1::minimum_y_projection_func(device, dilated_touch_portion_matrix_0_set_to_high, result_device_vector);
    result_device_vector->readTo(minimum_touch_portion_dilated_r.data());
    auto & maximum_touch_portion_dilated_r = nei_stats["maximum_touch_portion_dilated_r_" + std::to_string(r)];
    maximum_touch_portion_dilated_r.resize(nb_labels);
    tier1::maximum_y_projection_func(device, dilated_touch_portion_matrix, result_device_vector);
    result_device_vector->readTo(maximum_touch_portion_dilated_r.data());
  }

  return nei_stats;
}

} // namespace cle
