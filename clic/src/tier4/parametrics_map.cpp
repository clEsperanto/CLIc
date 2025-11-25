
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
parametric_map_func(const Device::Pointer & device,
  const Array::Pointer & labels,
                Array::Pointer intensity,
               const std::string & property,
               Array::Pointer dst) -> Array::Pointer
{
  if(intensity == nullptr)
  {
    intensity = labels;
  }

  tier0::create_like(labels, dst, dType::FLOAT);
  auto props = tier3::statistics_of_background_and_labelled_pixels_func(device, intensity, labels);

  // force property name to lower case
  std::string lower_property_name;
  std::transform(property.begin(), property.end(), std::back_inserter(lower_property_name), ::tolower);
  
  // Check if property exists
  if (props.find(lower_property_name) == props.end()) {
    throw std::runtime_error("Property '" + property + "' not found in statistics");
  }
  
  auto & vector = props[lower_property_name];
  auto values = Array::create(vector.size(), 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  values->writeFrom(vector.data());
  
  tier1::set_column_func(device, values, 0, 0);
  return tier1::replace_values_func(device, labels, values, dst);
}

auto
pixel_count_map_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return parametric_map_func(device, src, nullptr, std::string("area"), dst);
}

auto
label_pixel_count_map_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return pixel_count_map_func(device, src, dst);
}

auto
extension_ratio_map_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return parametric_map_func(device, src, nullptr, std::string("mean_max_distance_to_centroid_ratio"), dst);
}

auto
mean_extension_map_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return parametric_map_func(device, src, nullptr, std::string("mean_distance_to_centroid"), dst);
}

auto
maximum_extension_map_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return parametric_map_func(device, src, nullptr, std::string("max_distance_to_centroid"), dst);
}


auto
mean_intensity_map_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & labels, Array::Pointer dst)
  -> Array::Pointer
{
  return parametric_map_func(device, src, labels, std::string("mean_intensity"), dst);
}

auto
label_mean_intensity_map_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & labels, Array::Pointer dst)
  -> Array::Pointer
{
  return mean_intensity_map_func(device, src, labels, dst);
}

auto
minimum_intensity_map_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & labels, Array::Pointer dst)
  -> Array::Pointer
{
  return parametric_map_func(device, src, labels, std::string("min_intensity"), dst);
}

auto
maximum_intensity_map_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & labels, Array::Pointer dst)
  -> Array::Pointer
{
  return parametric_map_func(device, src, labels, std::string("max_intensity"), dst);
}

auto
standard_deviation_intensity_map_func(const Device::Pointer & device,
                                      const Array::Pointer &  src,
                                      const Array::Pointer &  labels,
                                      Array::Pointer          dst) -> Array::Pointer
{
  return parametric_map_func(device, src, labels, std::string("standard_deviation_intensity"), dst);
}

auto
touching_neighbor_count_map_func(const Device::Pointer & device, const Array::Pointer & labels, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(labels, dst, dType::FLOAT);
  auto touch_matrix = tier3::generate_touch_matrix_func(device, labels, nullptr);
  tier1::set_column_func(device, touch_matrix, 0, 0);
  auto nb_touching_neighbors = tier2::count_touching_neighbors_func(device, touch_matrix, nullptr, true);
  return tier1::replace_values_func(device, labels, nb_touching_neighbors, dst);
}


namespace
{
template <typename OperationFunc>
auto
apply_touching_neighbors_operation(const Device::Pointer & device,
                                   const Array::Pointer &  map,
                                   const Array::Pointer &  labels,
                                   Array::Pointer &        dst,
                                   int                     radius,
                                   bool                    ignore_background,
                                   OperationFunc           operation) -> Array::Pointer
{
  tier0::create_like(map, dst, dType::FLOAT);
  if (radius < 1)
  {
    tier1::copy_func(device, map, dst);
    return dst;
  }
  auto touch_matrix = tier3::generate_touch_matrix_func(device, labels, nullptr);
  if (ignore_background)
  {
    tier1::set_column_func(device, touch_matrix, 0, 0);
  }
  for (int r = 1; r < radius; r++)
  {
    touch_matrix = tier3::generate_touch_matrix_func(device, touch_matrix, nullptr);
    if (ignore_background)
    {
      tier1::set_column_func(device, touch_matrix, 0, 0);
    }
  }
  auto values = tier3::read_map_values_func(device, map, labels, nullptr);
  auto new_values = operation(device, values, touch_matrix, nullptr);
  tier1::set_column_func(device, new_values, 0, 0);
  tier1::replace_intensities_func(device, labels, new_values, dst);
  return dst;
}
} // anonymous namespace


auto
mean_of_touching_neighbors_map_func(const Device::Pointer & device,
                                    const Array::Pointer &  map,
                                    const Array::Pointer &  labels,
                                    Array::Pointer          dst,
                                    int                     radius,
                                    bool                    ignore_background) -> Array::Pointer
{
  return apply_touching_neighbors_operation(device, map, labels, dst, radius, ignore_background, tier1::mean_of_touching_neighbors_func);
}


auto
minimum_of_touching_neighbors_map_func(const Device::Pointer & device,
                                       const Array::Pointer &  map,
                                       const Array::Pointer &  labels,
                                       Array::Pointer          dst,
                                       int                     radius,
                                       bool                    ignore_background) -> Array::Pointer
{
  return apply_touching_neighbors_operation(device, map, labels, dst, radius, ignore_background, tier1::minimum_of_touching_neighbors_func);
}


auto
maximum_of_touching_neighbors_map_func(const Device::Pointer & device,
                                       const Array::Pointer &  map,
                                       const Array::Pointer &  labels,
                                       Array::Pointer          dst,
                                       int                     radius,
                                       bool                    ignore_background) -> Array::Pointer
{
  return apply_touching_neighbors_operation(device, map, labels, dst, radius, ignore_background, tier1::maximum_of_touching_neighbors_func);
}


auto
standard_deviation_of_touching_neighbors_map_func(const Device::Pointer & device,
                                                  const Array::Pointer &  map,
                                                  const Array::Pointer &  labels,
                                                  Array::Pointer          dst,
                                                  int                     radius,
                                                  bool                    ignore_background) -> Array::Pointer
{
  return apply_touching_neighbors_operation(
    device, map, labels, dst, radius, ignore_background, tier1::standard_deviation_of_touching_neighbors_func);
}


auto
mode_of_touching_neighbors_map_func(const Device::Pointer & device,
                                    const Array::Pointer &  map,
                                    const Array::Pointer &  labels,
                                    Array::Pointer          dst,
                                    int                     radius,
                                    bool                    ignore_background) -> Array::Pointer
{
  return apply_touching_neighbors_operation(device, map, labels, dst, radius, ignore_background, tier1::mode_of_touching_neighbors_func);
}


auto
median_of_touching_neighbors_map_func(const Device::Pointer & device,
                                      const Array::Pointer &  map,
                                      const Array::Pointer &  labels,
                                      Array::Pointer          dst,
                                      int                     radius,
                                      bool                    ignore_background) -> Array::Pointer
{
  return apply_touching_neighbors_operation(device, map, labels, dst, radius, ignore_background, tier1::median_of_touching_neighbors_func);
}

} // namespace cle::tier4
