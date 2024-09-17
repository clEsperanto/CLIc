#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"

#include "utils.hpp"

namespace cle::tier4
{

/**
 * @name label_pixel_count_map
 * @brief Takes a label map, determines the number of pixels per label and replaces every label with the that number.
 * This results in a parametric image expressing area or volume.
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src Label image to measure [const Array::Pointer &]
 * @param dst Parametric image computed[Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'label measurement', 'map', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_pixelCountMap
 */
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


} // namespace cle::tier4
