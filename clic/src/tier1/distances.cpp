#include "tier0.hpp"
#include "tier1.hpp"
#include "utils.hpp"

#include "cle_generate_distance_matrix.h"
#include "cle_maximum_distance_n_nearest_neighbors.h"
#include "cle_maximum_distance_touching_neighbors.h"
#include "cle_mean_distance_n_farthest_neighbors.h"
#include "cle_mean_distance_n_nearest_neighbors.h"
#include "cle_mean_distance_touching_neighbors.h"
#include "cle_minimum_distance_touching_neighbors.h"

namespace cle::tier1
{

auto
generate_distance_matrix_func(const Device::Pointer & device,
                              const Array::Pointer &  coordinate_list1,
                              const Array::Pointer &  coordinate_list2,
                              Array::Pointer          distance_matrix_destination) -> Array::Pointer
{
  tier0::create_dst(
    coordinate_list1, distance_matrix_destination, coordinate_list1->width() + 1, coordinate_list1->width() + 1, 1, dType::FLOAT);
  distance_matrix_destination->fill(0);
  const KernelInfo    kernel = { "generate_distance_matrix", kernel::generate_distance_matrix };
  const ParameterList params = { { "src0", coordinate_list1 }, { "src1", coordinate_list2 }, { "dst", distance_matrix_destination } };
  const RangeArray    range = { distance_matrix_destination->width(),
                                distance_matrix_destination->height(),
                                distance_matrix_destination->depth() };
  execute(device, kernel, params, range);
  return distance_matrix_destination;
}

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
