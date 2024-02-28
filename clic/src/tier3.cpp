#include "tier3.hpp"
#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "cle_exclude_on_edges.h"
#include "cle_flag_existing_labels.h"
#include "cle_generate_binary_overlap_matrix.h"
#include "cle_generate_touch_matrix.h"
#include "cle_histogram.h"
#include "cle_labelled_spots_to_point_list.h"

namespace cle::tier3
{

auto
bounding_box_func(const Device::Pointer & device, const Array::Pointer & src) -> std::array<float, 6>
{
  float min_x, min_y, min_z, max_x, max_y, max_z = 1;
  auto  temp = tier1::multiply_image_and_position_func(device, src, nullptr, 0);
  max_x = tier2::maximum_of_all_pixels_func(device, temp);
  min_x = tier2::minimum_of_masked_pixels_func(device, temp, src);
  temp = tier1::multiply_image_and_position_func(device, src, nullptr, 1);
  max_y = tier2::maximum_of_all_pixels_func(device, temp);
  min_y = tier2::minimum_of_masked_pixels_func(device, temp, src);
  if (src->depth() > 1)
  {
    temp = tier1::multiply_image_and_position_func(device, src, nullptr, 2);
    max_z = tier2::maximum_of_all_pixels_func(device, temp);
    min_z = tier2::minimum_of_masked_pixels_func(device, temp, src);
  }
  return std::array<float, 6>{ min_x, min_y, min_z, max_x, max_y, max_z };
}

auto
center_of_mass_func(const Device::Pointer & device, const Array::Pointer & src) -> std::array<float, 3>
{
  auto sum = tier2::sum_of_all_pixels_func(device, src);
  auto temp = tier1::multiply_image_and_position_func(device, src, nullptr, 0);
  auto sum_x = tier2::sum_of_all_pixels_func(device, temp);
  temp = tier1::multiply_image_and_position_func(device, src, nullptr, 1);
  auto sum_y = tier2::sum_of_all_pixels_func(device, temp);
  temp = tier1::multiply_image_and_position_func(device, src, nullptr, 2);
  auto sum_z = tier2::sum_of_all_pixels_func(device, temp);
  return std::array<float, 3>{ sum_x / sum, sum_y / sum, sum_z / sum };
}

// auto proximal_other_labels_count_func


auto
exclude_labels_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    const Array::Pointer &  list,
                    Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  if (list->dtype() != dType::LABEL)
  {
    throw std::runtime_error("exclude_labels: label list must be of type uint32");
  }
  std::vector<unsigned int> labels_list(list->size());
  list->read(labels_list.data());

  labels_list.front() = 0;
  unsigned int count = 1;
  for (int i = 1; i < labels_list.size(); i++)
  {
    if (labels_list[i] == 0)
    {
      labels_list[i] = count;
      count++;
    }
    else
    {
      labels_list[i] = 0;
    }
  }

  auto index_list = Array::create(list->size(), 1, 1, 1, dType::UINT32, mType::BUFFER, src->device());
  index_list->write(labels_list.data());
  tier1::replace_values_func(device, src, index_list, dst);
  return dst;
}

auto
exclude_labels_on_edges_func(const Device::Pointer & device,
                             const Array::Pointer &  src,
                             Array::Pointer          dst,
                             bool                    exclude_x,
                             bool                    exclude_y,
                             bool                    exclude_z) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::LABEL);
  auto num_labels = static_cast<uint32_t>(tier2::maximum_of_all_pixels_func(device, src));
  auto label_map = Array::create(num_labels + 1, 1, 1, 1, dType::LABEL, mType::BUFFER, src->device());
  tier1::set_ramp_x_func(device, label_map);

  const ParameterList params = { { "src", src }, { "dst", label_map } };
  if (exclude_x && src->width() > 1)
  {
    const KernelInfo kernel = { "exclude_on_edges_x", kernel::exclude_on_edges };
    const RangeArray range = { 1, src->height(), src->depth() };
    execute(device, kernel, params, range);
  }
  if (exclude_y && src->height() > 1)
  {
    const KernelInfo kernel = { "exclude_on_edges_y", kernel::exclude_on_edges };
    const RangeArray range = { src->width(), 1, src->depth() };
    execute(device, kernel, params, range);
  }
  if (exclude_z && src->depth() > 1)
  {
    const KernelInfo kernel = { "exclude_on_edges_z", kernel::exclude_on_edges };
    const RangeArray range = { src->width(), src->height(), 1 };
    execute(device, kernel, params, range);
  }
  std::vector<uint32_t> label_map_vector(label_map->size());
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
  return tier1::replace_values_func(device, src, label_map, dst);
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
  tier0::create_vector(src, dst, max + 1, dType::LABEL);
  dst->fill(0);
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

auto
generate_binary_overlap_matrix_func(const Device::Pointer & device,
                                    const Array::Pointer &  src0,
                                    const Array::Pointer &  src1,
                                    Array::Pointer          dst) -> Array::Pointer
{
  if (dst == nullptr)
  {
    auto max_label_0 = tier2::maximum_of_all_pixels_func(device, src0) + 1;
    auto max_label_1 = tier2::maximum_of_all_pixels_func(device, src1) + 1;
    tier0::create_dst(src0, dst, max_label_0, max_label_1, 1, dType::UINT32);
  }
  dst->fill(0);
  const KernelInfo    kernel = { "generate_binary_overlap_matrix", kernel::generate_binary_overlap_matrix };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { src0->width(), src0->height(), src0->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
generate_touch_matrix_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  if (dst == nullptr)
  {
    auto max_label = tier2::maximum_of_all_pixels_func(device, src) + 1;
    tier0::create_dst(src, dst, max_label, max_label, 1, dType::UINT32);
  }
  dst->fill(0);
  const KernelInfo    kernel = { "generate_touch_matrix", kernel::generate_touch_matrix };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return dst;
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
  auto   partial_hist =
    Array::create(nbins, 1, number_of_partial_histograms, 3, dType::UINT32, src->mtype(), src->device());
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

auto
jaccard_index_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1) -> float
{
  auto intersection_ = tier1::binary_and_func(device, src0, src1, nullptr);
  auto union_ = tier1::binary_or_func(device, src0, src1, nullptr);
  return tier2::sum_of_all_pixels_func(device, intersection_) / tier2::sum_of_all_pixels_func(device, union_);
}

auto
labelled_spots_to_pointlist_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  auto max_label = tier2::maximum_of_all_pixels_func(device, src);
  auto dim = shape_to_dimension(src->width(), src->height(), src->depth());
  tier0::create_dst(src, dst, max_label, dim, 1, dType::LABEL);
  dst->fill(0);

  const KernelInfo    kernel = { "labelled_spots_to_point_list", kernel::labelled_spots_to_point_list };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return dst;
}


// auto maximum_of_n_most_touching_neighbors_map_func
// auto maximum_of_n_nearest_neighbors_map_func
// auto maximum_of_touch_portion_within_range_neighbors_map_func
// auto maximum_of_touching_neighbors_map_func
// auto maximum_of_proximal_neighbors_map_func

auto
maximum_position_func(const Device::Pointer & device, const Array::Pointer & src) -> std::array<size_t, 3>
{
  size_t                z_coord = 0;
  size_t                y_coord = 0;
  size_t                x_coord = 0;
  std::array<size_t, 3> coord = { 0, 0, 0 };

  Array::Pointer pos_x;
  Array::Pointer pos_y;
  Array::Pointer pos_z;
  Array::Pointer temp = src;

  if (src->depth() > 1)
  {
    pos_z = tier1::z_position_of_maximum_z_projection_func(device, temp, nullptr);
    temp = tier1::maximum_z_projection_func(device, temp, nullptr);
  }
  if (src->height() > 1)
  {
    pos_y = tier1::y_position_of_maximum_y_projection_func(device, temp, nullptr);
    temp = tier1::maximum_y_projection_func(device, temp, nullptr);
  }
  pos_x = tier1::x_position_of_maximum_x_projection_func(device, temp, nullptr);
  temp = tier1::maximum_x_projection_func(device, temp, nullptr);


  if (pos_x != nullptr)
  {
    pos_x->read(&x_coord, { 1, 1, 1 }, { 0, 0, 0 });
    coord[0] = x_coord;
  }
  if (pos_y != nullptr)
  {
    pos_y->read(&y_coord, { 1, 1, 1 }, { x_coord, 0, 0 });
    coord[1] = y_coord;
  }
  if (pos_z != nullptr)
  {
    pos_z->read(&z_coord, { 1, 1, 1 }, { x_coord, y_coord, 0 });
    coord[2] = z_coord;
  }
  return coord;
}

auto
mean_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float
{
  auto temp = tier2::sum_of_all_pixels_func(device, src);
  return temp / src->size();
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

auto
minimum_position_func(const Device::Pointer & device, const Array::Pointer & src) -> std::array<size_t, 3>
{
  size_t                z_coord = 0;
  size_t                y_coord = 0;
  size_t                x_coord = 0;
  std::array<size_t, 3> coord = { 0, 0, 0 };

  Array::Pointer pos_x;
  Array::Pointer pos_y;
  Array::Pointer pos_z;
  Array::Pointer temp = src;

  if (src->depth() > 1)
  {
    pos_z = tier1::z_position_of_minimum_z_projection_func(device, temp, nullptr);
    temp = tier1::minimum_z_projection_func(device, temp, nullptr);
  }
  if (src->height() > 1)
  {
    pos_y = tier1::y_position_of_minimum_y_projection_func(device, temp, nullptr);
    temp = tier1::minimum_y_projection_func(device, temp, nullptr);
  }
  pos_x = tier1::x_position_of_minimum_x_projection_func(device, temp, nullptr);
  temp = tier1::minimum_x_projection_func(device, temp, nullptr);


  if (pos_x != nullptr)
  {
    pos_x->read(&x_coord, { 1, 1, 1 }, { 0, 0, 0 });
    coord[0] = x_coord;
  }
  if (pos_y != nullptr)
  {
    pos_y->read(&y_coord, { 1, 1, 1 }, { x_coord, 0, 0 });
    coord[1] = y_coord;
  }
  if (pos_z != nullptr)
  {
    pos_z->read(&z_coord, { 1, 1, 1 }, { x_coord, y_coord, 0 });
    coord[2] = z_coord;
  }
  return coord;
}


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
// auto z_position_range_projection_func

} // namespace cle::tier3
