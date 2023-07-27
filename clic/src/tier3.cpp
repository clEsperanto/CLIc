#include "tier3.hpp"
#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "cle_exclude_on_edges.h"
#include "cle_flag_existing_labels.h"
#include "cle_histogram.h"

namespace cle::tier3
{

// auto bounding_box_func
// auto center_of_mass_func
// auto proximal_other_labels_count_func
// auto divide_by_gaussian_background_func
// auto exclude_labels_func

auto
exclude_labels_on_edges_func(const Device::Pointer & device,
                             const Array::Pointer &  src,
                             Array::Pointer          dst,
                             bool                    exclude_y,
                             bool                    exclude_x,
                             bool                    exclude_z) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::UINT32);
  auto num_labels = static_cast<int>(tier2::maximum_of_all_pixels_func(device, src));
  auto label_map = Array::create(num_labels + 1, 1, 1, dType::UINT32, mType::BUFFER, src->device());
  tier1::set_ramp_x_func(device, label_map);

  const ParameterList params = { { "src", src }, { "dst", label_map } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  if (exclude_x)
  {
    const KernelInfo kernel = { "exclude_on_edges_x", kernel::exclude_on_edges };
    execute(device, kernel, params, range);
  }
  if (exclude_y)
  {
    const KernelInfo kernel = { "exclude_on_edges_y", kernel::exclude_on_edges };
    execute(device, kernel, params, range);
  }
  if (exclude_z)
  {
    const KernelInfo kernel = { "exclude_on_edges_z", kernel::exclude_on_edges };
    execute(device, kernel, params, range);
  }
  std::vector<int> label_map_vector(label_map->nbElements());
  label_map->read(label_map_vector.data());
  int count = 1;
  for (auto & i : label_map_vector)
  {
    if (i > 0)
    {
      i = count;
      count++;
    }
  }
  label_map->write(label_map_vector.data());
  return tier1::replace_intensities_func(device, src, label_map, dst);
}

// auto exclude_labels_with_values_equal_to_constant_func
// auto exclude_labels_with_values_not_equal_to_constant_func
// auto exclude_labels_outside_size_range_func
// auto exclude_labels_with_values_out_of_range_func
// auto exclude_labels_with_values_within_range_func

auto
flag_existing_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  auto max = tier2::maximum_of_all_pixels_func(device, src);
  tier0::create_vector(src, dst, max + 1, dType::UINT32);
  const KernelInfo    kernel = { "flag_existing_labels", kernel::flag_existing_labels };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
gamma_correction_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float gamma)
  -> Array::Pointer
{
  auto max_intensity = tier2::maximum_of_all_pixels_func(device, src);
  auto temp1 = tier1::multiply_image_and_scalar_func(device, src, nullptr, 1.0 / max_intensity);
  auto temp2 = tier1::power_func(device, temp1, nullptr, gamma);
  return tier1::multiply_image_and_scalar_func(device, temp2, dst, max_intensity);
}

// auto generate_n_nearest_neighbors_matrix_func
// auto generate_proximal_neighbors_matrix_func
// auto generate_touch_count_matrix_func
// auto generate_touch_mean_intensity_matrix_func

auto
histogram_func(const Device::Pointer & device,
               const Array::Pointer &  src,
               Array::Pointer          dst,
               int                     nbins,
               float                   min,
               float                   max) -> Array::Pointer
{
  tier0::create_vector(src, dst, nbins);
  size_t number_of_partial_histograms = src->height();
  auto partial_hist = Array::create(nbins, 1, number_of_partial_histograms, dType::UINT32, src->mtype(), src->device());
  if (std::isnan(max) || std::isnan(max))
  {
    min = tier2::minimum_of_all_pixels_func(device, src);
    max = tier2::maximum_of_all_pixels_func(device, src);
  }
  const KernelInfo    kernel = { "histogram", kernel::histogram };
  const ParameterList params = { { "src", src },       { "dst", partial_hist }, { "minimum", min },  { "maximum", max },
                                 { "step_size_x", 1 }, { "step_size_y", 1 },    { "step_size_z", 1 } };
  const ConstantList  consts = { { "NUMBER_OF_HISTOGRAM_BINS", nbins } };
  const RangeArray    range = { number_of_partial_histograms, 1, 1 };
  execute(device, kernel, params, range, consts);
  return tier1::sum_z_projection_func(device, partial_hist, dst);
}


// auto jaccard_index_func
// auto labelled_spots_to_pointlist_func
// auto maximum_of_n_most_touching_neighbors_map_func
// auto maximum_of_n_nearest_neighbors_map_func
// auto maximum_of_touch_portion_within_range_neighbors_map_func
// auto maximum_of_touching_neighbors_map_func
// auto maximum_of_proximal_neighbors_map_func
// auto maximum_position_func

auto
mean_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float
{
  auto temp = tier2::sum_of_all_pixels_func(device, src);
  return temp / src->nbElements();
}

// auto mean_of_n_most_touching_neighbors_map_func
// auto mean_of_n_nearest_neighbors_map_func
// auto mean_of_proximal_neighbors_map_func
// auto mean_of_touch_portion_within_range_neighbors_map_func
// auto mean_of_touching_neighbors_map_func


// auto minimum_of_n_most_touching_neighbors_map_func
// auto minimum_of_n_nearest_neighbors_map_func
// auto minimum_of_proximal_neighbors_map_func
// auto minimum_of_touch_portion_within_range_neighbors_map_func
// auto minimum_of_touching_neighbors_map_func
// auto minimum_position_func
// auto mode_of_n_most_touching_neighbors_map_func
// auto mode_of_n_nearest_neighbors_map_func
// auto mode_of_proximal_neighbors_map_func
// auto mode_of_touch_portion_within_range_neighbors_map_func
// auto mode_of_touching_neighbors_map_func
// auto standard_deviation_of_n_most_touching_neighbors_map_func
// auto standard_deviation_of_n_nearest_neighbors_map_func
// auto standard_deviation_of_touch_portion_within_range_neighbors_map_func
// auto standard_deviation_of_touching_neighbors_map_func
// auto standard_deviation_of_proximal_neighbors_map_func
// auto subtract_gaussian_background_func
// auto z_position_range_projection_func

} // namespace cle::tier3
