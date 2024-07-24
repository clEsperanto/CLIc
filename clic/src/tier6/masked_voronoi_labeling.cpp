#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"
#include "tier6.hpp"

#include "utils.hpp"

namespace cle::tier6
{

auto
masked_voronoi_labeling_func(const Device::Pointer & device,
                             const Array::Pointer &  src,
                             const Array::Pointer &  mask,
                             Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::LABEL);
  Array::Pointer flip = nullptr;
  Array::Pointer flop = nullptr;
  Array::Pointer flup = nullptr;
  tier0::create_like(src, flip, dType::FLOAT);
  tier0::create_like(src, flop, dType::FLOAT);
  tier0::create_like(src, flup, dType::FLOAT);

  tier1::add_image_and_scalar_func(device, mask, flup, -1);
  tier5::connected_components_labeling_func(device, src, flop, "box");
  tier1::add_images_weighted_func(device, flop, flup, flip, 1, 1);
  auto flag = Array::create(1, 1, 1, 1, dType::INT32, mType::BUFFER, device);
  flag->fill(1);

  int flag_value = 1;
  int iter_count = 0;
  while (flag_value > 0)
  {
    auto active = (iter_count % 2 == 0) ? flip : flop;
    auto passive = (iter_count % 2 == 0) ? flop : flip;
    tier1::onlyzero_overwrite_maximum_func(device, active, flag, passive, (iter_count % 2 == 0) ? "box" : "sphere");
    flag->readTo(&flag_value);
    if (flag_value > 0)
    {
      flag->fill(0);
    }
    iter_count++;
  }

  return tier1::mask_func(device, (iter_count % 2 == 0) ? flip : flop, mask, dst);
}

} // namespace cle::tier6
