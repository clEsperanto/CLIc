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
connected_component_labeling_func(const Device::Pointer & device,
                                   const Array::Pointer &  src,
                                   Array::Pointer          dst,
                                   const std::string &     connectivity) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::LABEL);
  auto temp1 = tier1::set_nonzero_pixels_to_pixelindex_func(device, src, nullptr, 1);
  auto temp2 = Array::create(temp1);
  temp2->fill(0);

  auto flag = Array::create(1, 1, 1, 1, dType::INT32, mType::BUFFER, device);
  flag->fill(0);

  int flag_value = 1;
  int iter_count = 0;
  while (flag_value > 0)
  {
    auto active = (iter_count % 2 == 0) ? temp1 : temp2;
    auto passive = (iter_count % 2 == 0) ? temp2 : temp1;
    tier1::nonzero_minimum_func(device, active, flag, passive, connectivity);
    flag->readTo(&flag_value);
    if (flag_value > 0)
    {
      flag->fill(0);
    }
    iter_count++;
  }
  return tier4::relabel_sequential_func(device, (iter_count % 2 == 0) ? temp1 : temp2, dst, 4096);
}

auto
connected_components_labeling_func(const Device::Pointer & device,
                                   const Array::Pointer &  src,
                                   Array::Pointer          dst,
                                   const std::string &     connectivity) -> Array::Pointer
{
  return connected_component_labeling_func(device, src, dst, connectivity);
}

} // namespace cle::tier5
