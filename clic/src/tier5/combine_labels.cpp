#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"

#include "utils.hpp"

namespace cle::tier5
{

auto
combine_labels_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_like(src0, dst, dType::LABEL);
  auto max_label = tier2::maximum_of_all_pixels_func(device, src0);
  auto temp1 = tier1::add_image_and_scalar_func(device, src1, nullptr, max_label);
  auto temp2 = tier1::greater_constant_func(device, src1, nullptr, 0);
  temp1 = tier1::mask_func(device, temp1, temp2, nullptr);
  temp2 = tier1::maximum_images_func(device, src0, temp1, nullptr);
  return tier4::relabel_sequential_func(device, temp2, dst, 4096);
}

} // namespace cle::tier5
