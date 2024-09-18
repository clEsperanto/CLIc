
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
label_pixel_count_map_func(const Device::Pointer & device,
                           const Array::Pointer &  src,
                           Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  auto props = tier3::statistics_of_labelled_pixels_func(device, src, nullptr);

  for (auto && i : props["area"])
  {
    std::cout << i << std::endl;
  }


  auto values = cle::Array::create(props["area"].size(), 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  values->writeFrom(props["area"].data());

  cle::print<float>(values, "values");
  tier1::set_column_func(device, values, 0, 0);
  return tier1::replace_values_func(device, src, values, dst);
}

auto
extension_ratio_map_func(const Device::Pointer & device,
                         const Array::Pointer &  src,
                         Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  auto props = tier3::statistics_of_background_and_labelled_pixels_func(device, src, nullptr);
  auto vector = props["mean_max_distance_to_centroid_ratio"];
  auto values = Array::create(vector.size(), 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  values->writeFrom(vector.data());
  tier1::set_column_func(device, values, 0, 0);
  return tier1::replace_values_func(device, src, values, dst);
}


} // namespace cle::tier4
