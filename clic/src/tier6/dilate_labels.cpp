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
dilate_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int radius)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::LABEL);
  if (radius <= 0)
  {
    return tier1::copy_func(device, src, dst);
  }

  auto flip = tier1::copy_func(device, src, nullptr);
  auto flop = Array::create(flip);
  auto flag = Array::create(1, 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  flag->fill(0);

  int   iter_count = 0;
  float flag_value = 1;
  while (flag_value > 0 && iter_count < radius)
  {
    auto active = (iter_count % 2 == 0) ? flip : flop;
    auto passive = (iter_count % 2 == 0) ? flop : flip;
    tier1::onlyzero_overwrite_maximum_func(device, active, flag, passive, (iter_count % 2 == 0) ? "box" : "sphere");
    flag->read(&flag_value);
    if (flag_value > 0)
    {
      flag->fill(0);
    }
    iter_count++;
  }

  return tier1::copy_func(device, (iter_count % 2 == 0) ? flip : flop, dst);
}

} // namespace cle::tier6
