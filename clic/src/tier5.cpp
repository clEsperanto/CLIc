#include "tier5.hpp"
#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"

namespace cle::tier5
{

auto
combine_labels_func(const Device::Pointer & device,
                    const Array::Pointer &  src0,
                    const Array::Pointer &  src1,
                    Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src0, dst);
  auto max_label = tier2::maximum_of_all_pixels_func(device, src0);
  auto temp1 = tier1::add_image_and_scalar_func(device, src1, nullptr, max_label);
  auto temp2 = tier1::greater_constant_func(device, src1, nullptr, 0);
  temp1 = tier1::mask_func(device, temp1, temp2, nullptr);
  temp2 = tier1::maximum_images_func(device, src0, temp1, nullptr);
  return tier4::relabel_sequential_func(device, temp2, dst, 4096);
}

auto
connected_components_labeling_box_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::INT64);

  auto temp1 =Array::create(dst);
  auto temp2 =Array::create(dst);
  temp2->fill(0);

  tier1::set_nonzero_pixels_to_pixelindex_func(device, src, temp1, 1);

  auto flag = Array::create(1, 1, 1, dType::INT32, mType::BUFFER, device);
  flag->fill(0);

  int flag_value = 1;
  int iteration_count = 0;
  while (flag_value > 0)
  {
    if (iteration_count % 2 == 0)
    {
      tier1::nonzero_minimum_box_func(device, temp1, flag, temp2);
    }
    else
    {
      tier1::nonzero_minimum_box_func(device, temp2, flag, temp1);
    }
    flag->read(&flag_value);
    flag->fill(0);
    iteration_count++;
  }
  if (iteration_count % 2 == 0)
  {
    tier4::relabel_sequential_func(device, temp1, dst, 4096);
  }
  else
  {
    tier4::relabel_sequential_func(device, temp2, dst, 4096);
  }
  return dst;
}


} // namespace cle::tier5