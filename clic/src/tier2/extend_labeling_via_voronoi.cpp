#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

namespace cle::tier2
{

auto
extend_labeling_via_voronoi_func(const Device::Pointer & device,
                                 const Array::Pointer &  src,
                                 Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::LABEL);
  auto flip = Array::create(dst);
  auto flop = Array::create(dst);
  tier1::copy_func(device, src, flip);

  auto flag = Array::create(1, 1, 1, 1, dType::INT32, mType::BUFFER, device);
  flag->fill(0);
  int flag_value = 1;
  int iteration_count = 0;
  while (flag_value > 0)
  {
    if (iteration_count % 2 == 0)
    {
      tier1::onlyzero_overwrite_maximum_func(device, flip, flag, flop, "box");
    }
    else
    {
      tier1::onlyzero_overwrite_maximum_func(device, flop, flag, flip, "box");
    }
    flag->readTo(&flag_value);
    flag->fill(0);
    iteration_count++;
  }
  if (iteration_count % 2 == 0)
  {
    flip->copyTo(dst);
  }
  else
  {
    flop->copyTo(dst);
  }
  return dst;
}

} // namespace cle::tier2
