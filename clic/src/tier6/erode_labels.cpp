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
erode_labels_func(const Device::Pointer & device,
                  const Array::Pointer &  input_labels,
                  Array::Pointer          dst,
                  int                     radius,
                  bool                    relabel) -> Array::Pointer
{
  tier0::create_like(input_labels, dst, dType::LABEL);
  if (radius <= 0)
  {
    return tier1::copy_func(device, input_labels, dst);
  }

  auto temp = tier1::detect_label_edges_func(device, input_labels, nullptr);
  auto temp1 = tier1::binary_not_func(device, temp, nullptr);
  tier1::mask_func(device, input_labels, temp1, temp);
  temp1.reset();

  if (radius == 1)
  {
    tier1::copy_func(device, temp, dst);
  }
  for (auto i = 0; i < radius - 1; i++)
  {
    auto active = (i % 2 == 0) ? temp : dst;
    auto passive = (i % 2 == 0) ? dst : temp;
    tier1::minimum_filter_func(device, active, passive, 1, 1, 1, (i % 2 == 0) ? "sphere" : "box");
  }

  if (relabel)
  {
    if (radius % 2 != 0)
    {
      tier1::copy_func(device, temp, dst);
    }
    tier1::not_equal_constant_func(device, dst, temp, 0);
    tier5::connected_component_labeling_func(device, temp, dst, "sphere");
  }
  else
  {
    if (radius % 2 == 0)
    {
      tier1::copy_func(device, dst, temp);
    }
    tier4::relabel_sequential_func(device, temp, dst, 4096);
  }
  return dst;
}

} // namespace cle::tier6
