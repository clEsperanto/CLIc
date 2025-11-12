
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
pixel_count_map_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  auto props = tier3::statistics_of_background_and_labelled_pixels_func(device, src, src);

  auto values = cle::Array::create(props["area"].size(), 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  values->writeFrom(props["area"].data());

  tier1::set_column_func(device, values, 0, 0);
  return tier1::replace_values_func(device, src, values, dst);
}

auto
label_pixel_count_map_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return pixel_count_map_func(device, src, dst);
}

auto
extension_ratio_map_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  auto props = tier3::statistics_of_background_and_labelled_pixels_func(device, src, src);
  auto vector = props["mean_max_distance_to_centroid_ratio"];
  auto values = Array::create(vector.size(), 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  values->writeFrom(vector.data());
  tier1::set_column_func(device, values, 0, 0);
  return tier1::replace_values_func(device, src, values, dst);
}

auto
mean_extension_map_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  auto props = tier3::statistics_of_background_and_labelled_pixels_func(device, src, src);
  auto vector = props["mean_distance_to_centroid"];
  auto values = Array::create(vector.size(), 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  values->writeFrom(vector.data());
  tier1::set_column_func(device, values, 0, 0);
  return tier1::replace_values_func(device, src, values, dst);
}

auto
maximum_extension_map_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  auto props = tier3::statistics_of_background_and_labelled_pixels_func(device, src, src);
  auto vector = props["max_distance_to_centroid"];
  auto values = Array::create(vector.size(), 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  values->writeFrom(vector.data());
  tier1::set_column_func(device, values, 0, 0);
  return tier1::replace_values_func(device, src, values, dst);
}


auto
mean_intensity_map_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & labels, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  auto props = tier3::statistics_of_background_and_labelled_pixels_func(device, src, labels);

  auto values = cle::Array::create(props["mean_intensity"].size(), 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  values->writeFrom(props["mean_intensity"].data());

  tier1::set_column_func(device, values, 0, 0);
  return tier1::replace_values_func(device, labels, values, dst);
}

auto
minimum_intensity_map_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & labels, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  auto props = tier3::statistics_of_background_and_labelled_pixels_func(device, src, labels);

  auto values = cle::Array::create(props["min_intensity"].size(), 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  values->writeFrom(props["min_intensity"].data());

  tier1::set_column_func(device, values, 0, 0);
  return tier1::replace_values_func(device, labels, values, dst);
}

auto
maximum_intensity_map_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & labels, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  auto props = tier3::statistics_of_background_and_labelled_pixels_func(device, src, labels);

  auto values = cle::Array::create(props["max_intensity"].size(), 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  values->writeFrom(props["max_intensity"].data());

  tier1::set_column_func(device, values, 0, 0);
  return tier1::replace_values_func(device, labels, values, dst);
}

auto
standard_deviation_intensity_map_func(const Device::Pointer & device,
                                      const Array::Pointer &  src,
                                      const Array::Pointer &  labels,
                                      Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  auto props = tier3::statistics_of_background_and_labelled_pixels_func(device, src, labels);

  auto values = cle::Array::create(props["std_intensity"].size(), 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  values->writeFrom(props["std_intensity"].data());

  tier1::set_column_func(device, values, 0, 0);
  return tier1::replace_values_func(device, labels, values, dst);
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
  template<typename OperationFunc>
  auto apply_touching_neighbors_operation(
    const Device::Pointer & device,
    const Array::Pointer & map,
    const Array::Pointer & labels,
    Array::Pointer & dst,
    int radius,
    bool ignore_background,
    OperationFunc operation) -> Array::Pointer
  {
    tier0::create_like(map, dst, dType::FLOAT);
    if(radius < 1)
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
mean_of_touching_neighbors_map_func(const Device::Pointer & device, const Array::Pointer & map, const Array::Pointer & labels, Array::Pointer dst, int radius, bool ignore_background) -> Array::Pointer
{
  return apply_touching_neighbors_operation(device, map, labels, dst, radius, ignore_background, tier1::mean_of_touching_neighbors_func);
}


auto
minimum_of_touching_neighbors_map_func(const Device::Pointer & device, const Array::Pointer & map, const Array::Pointer & labels, Array::Pointer dst, int radius, bool ignore_background)
-> Array::Pointer
{
  return apply_touching_neighbors_operation(device, map, labels, dst, radius, ignore_background, tier1::minimum_of_touching_neighbors_func);
}


auto
maximum_of_touching_neighbors_map_func(const Device::Pointer & device, const Array::Pointer & map, const Array::Pointer & labels, Array::Pointer dst, int radius, bool ignore_background)
-> Array::Pointer
{
  return apply_touching_neighbors_operation(device, map, labels, dst, radius, ignore_background, tier1::maximum_of_touching_neighbors_func);
}


auto
standard_deviation_of_touching_neighbors_map_func(const Device::Pointer & device, const Array::Pointer & map, const Array::Pointer & labels, Array::Pointer dst, int radius, bool ignore_background)
-> Array::Pointer
{
  return apply_touching_neighbors_operation(device, map, labels, dst, radius, ignore_background, tier1::standard_deviation_of_touching_neighbors_func);
}


auto
mode_of_touching_neighbors_map_func(const Device::Pointer & device, const Array::Pointer & map, const Array::Pointer & labels, Array::Pointer dst, int radius, bool ignore_background) -> Array::Pointer
{
  return apply_touching_neighbors_operation(device, map, labels, dst, radius, ignore_background, tier1::mode_of_touching_neighbors_func);
}


auto
median_of_touching_neighbors_map_func(const Device::Pointer & device, const Array::Pointer & map, const Array::Pointer & labels, Array::Pointer dst, int radius, bool ignore_background) -> Array::Pointer
{
  return apply_touching_neighbors_operation(device, map, labels, dst, radius, ignore_background, tier1::median_of_touching_neighbors_func);
}

} // namespace cle::tier4
