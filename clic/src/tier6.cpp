
#include "tier6.hpp"
#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"

namespace cle::tier6
{

auto
masked_voronoi_labeling_func(const Device::Pointer & device,
                             const Array::Pointer &  src,
                             const Array::Pointer &  mask,
                             Array::Pointer          dst) -> Array::Pointer
{

  tier0::create_like(src, dst);
  auto flup = tier1::add_image_and_scalar_func(device, mask, nullptr, -1);
  auto flop = tier5::connected_components_labeling_box_func(device, src, nullptr);
  auto flip = tier1::add_images_weighted_func(device, flup, flop, nullptr, 1, 1);
  auto flag = Array::create(1, 1, 1, dType::INT32, mType::BUFFER, device);
  flag->fill(0);
  int flag_value = 1;
  int iteration_count = 0;
  while (flag_value > 0)
  {
    if (iteration_count % 2 == 0)
    {
      tier1::onlyzero_overwrite_maximum_box_func(device, flip, flag, flop);
    }
    else
    {
      tier1::onlyzero_overwrite_maximum_box_func(device, flop, flag, flip);
    }
    flag->read(&flag_value);
    flag->fill(0);
    iteration_count++;
  }
  if (iteration_count % 2 == 0)
  {
    tier1::mask_func(device, flip, mask, dst);
  }
  else
  {
    tier1::mask_func(device, flop, mask, dst);
  }
  return dst;
}


} // namespace cle::tier6
