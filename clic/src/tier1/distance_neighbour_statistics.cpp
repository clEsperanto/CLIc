#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_maximum_distance_touching_neighbors.h"
#include "cle_mean_distance_touching_neighbors.h"
#include "cle_minimum_distance_touching_neighbors.h"

#include "cle_maximum_distance_n_nearest_neighbors.h"
#include "cle_mean_distance_n_farthest_neighbors.h"
#include "cle_mean_distance_n_nearest_neighbors.h"

namespace cle::tier1
{

auto
minimum_distance_touching_neighbors_func(const Device::Pointer & device,
                                         const Array::Pointer &  src_distance_matrix,
                                         const Array::Pointer &  src_touch_matrix,
                                         Array::Pointer          dst_index_list) -> Array::Pointer
{
  tier0::create_vector(src_distance_matrix, dst_index_list, src_distance_matrix->width(), dType::FLOAT);
  const KernelInfo    kernel = { "minimum_distance_touching_neighbors", kernel::minimum_distance_touching_neighbors };
  const ParameterList params = { { "src_distance_matrix", src_distance_matrix },
                                 { "src_touch_matrix", src_touch_matrix },
                                 { "dst_index_list", dst_index_list } };
  const RangeArray    range = { dst_index_list->width(), dst_index_list->height(), dst_index_list->depth() };
  execute(device, kernel, params, range);
  return dst_index_list;
}

auto
maximum_distance_touching_neighbors_func(const Device::Pointer & device,
                                         const Array::Pointer &  src_distance_matrix,
                                         const Array::Pointer &  src_touch_matrix,
                                         Array::Pointer          dst_index_list) -> Array::Pointer
{
  tier0::create_vector(src_distance_matrix, dst_index_list, src_distance_matrix->width(), dType::FLOAT);
  const KernelInfo    kernel = { "maximum_distance_touching_neighbors", kernel::maximum_distance_touching_neighbors };
  const ParameterList params = { { "src_distance_matrix", src_distance_matrix },
                                 { "src_touch_matrix", src_touch_matrix },
                                 { "dst_index_list", dst_index_list } };
  const RangeArray    range = { dst_index_list->width(), dst_index_list->height(), dst_index_list->depth() };
  execute(device, kernel, params, range);
  return dst_index_list;
}

auto
mean_distance_touching_neighbors_func(const Device::Pointer & device,
                                      const Array::Pointer &  src_distance_matrix,
                                      const Array::Pointer &  src_touch_matrix,
                                      Array::Pointer          dst_index_list) -> Array::Pointer
{
  tier0::create_vector(src_distance_matrix, dst_index_list, src_distance_matrix->width(), dType::FLOAT);
  const KernelInfo    kernel = { "mean_distance_touching_neighbors", kernel::mean_distance_touching_neighbors };
  const ParameterList params = { { "src_distance_matrix", src_distance_matrix },
                                 { "src_touch_matrix", src_touch_matrix },
                                 { "dst_index_list", dst_index_list } };
  const RangeArray    range = { dst_index_list->width(), dst_index_list->height(), dst_index_list->depth() };
  execute(device, kernel, params, range);
  return dst_index_list;
}

auto
average_distance_touching_neighbors_func(const Device::Pointer & device,
                                         const Array::Pointer &  src_distance_matrix,
                                         const Array::Pointer &  src_touch_matrix,
                                         Array::Pointer          dst_index_list) -> Array::Pointer
{
  return mean_distance_touching_neighbors_func(device, src_distance_matrix, src_touch_matrix, dst_index_list);
}


auto
mean_distance_n_nearest_neighbors_func(const Device::Pointer & device,
                                       const Array::Pointer &  src_distance_matrix,
                                       Array::Pointer          dst_index_list,
                                       int                     n) -> Array::Pointer
{
  tier0::create_vector(src_distance_matrix, dst_index_list, src_distance_matrix->width(), dType::FLOAT);
  const KernelInfo    kernel = { "mean_distance_n_nearest_neighbors", kernel::mean_distance_n_nearest_neighbors };
  const ParameterList params = { { "src_distance_matrix", src_distance_matrix }, { "dst_index_list", dst_index_list }, { "nPoints", n } };
  const RangeArray    range = { dst_index_list->width(), dst_index_list->height(), dst_index_list->depth() };
  execute(device, kernel, params, range);
  return dst_index_list;
}

auto
maximum_distance_n_nearest_neighbors_func(const Device::Pointer & device,
                                          const Array::Pointer &  src_distance_matrix,
                                          Array::Pointer          dst_index_list,
                                          int                     n) -> Array::Pointer
{
  tier0::create_vector(src_distance_matrix, dst_index_list, src_distance_matrix->width(), dType::FLOAT);
  const KernelInfo    kernel = { "maximum_distance_n_nearest_neighbors", kernel::maximum_distance_n_nearest_neighbors };
  const ParameterList params = { { "src_distance_matrix", src_distance_matrix }, { "dst_index_list", dst_index_list }, { "nPoints", n } };
  const RangeArray    range = { dst_index_list->width(), dst_index_list->height(), dst_index_list->depth() };
  execute(device, kernel, params, range);
  return dst_index_list;
}

auto
mean_distance_n_farthest_neighbors_func(const Device::Pointer & device,
                                        const Array::Pointer &  src_distance_matrix,
                                        Array::Pointer          dst_index_list,
                                        int                     n) -> Array::Pointer
{
  tier0::create_vector(src_distance_matrix, dst_index_list, src_distance_matrix->width(), dType::FLOAT);
  const KernelInfo    kernel = { "mean_distance_n_farthest_neighbors", kernel::mean_distance_n_farthest_neighbors };
  const ParameterList params = { { "src_distance_matrix", src_distance_matrix }, { "dst_index_list", dst_index_list }, { "nPoints", n } };
  const RangeArray    range = { dst_index_list->width(), dst_index_list->height(), dst_index_list->depth() };
  execute(device, kernel, params, range);
  return dst_index_list;
}

auto
average_distance_of_n_nearest_distances_func(const Device::Pointer & device,
                                             const Array::Pointer &  src_distance_matrix,
                                             Array::Pointer          dst_index_list,
                                             int                     n) -> Array::Pointer
{
  return mean_distance_n_nearest_neighbors_func(device, src_distance_matrix, dst_index_list, n);
}
auto
maximum_distance_of_n_shortest_distances_func(const Device::Pointer & device,
                                              const Array::Pointer &  src_distance_matrix,
                                              Array::Pointer          dst_index_list,
                                              int                     n) -> Array::Pointer
{
  return maximum_distance_n_nearest_neighbors_func(device, src_distance_matrix, dst_index_list, n);
}
auto
average_distance_of_n_far_off_distances_func(const Device::Pointer & device,
                                             const Array::Pointer &  src_distance_matrix,
                                             Array::Pointer          dst_index_list,
                                             int                     n) -> Array::Pointer
{
  return mean_distance_n_farthest_neighbors_func(device, src_distance_matrix, dst_index_list, n);
}

} // namespace cle::tier1
