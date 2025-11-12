#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

namespace cle::tier2
{

auto
reduce_labels_to_label_edges_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  auto binary = tier1::detect_label_edges_func(device, src, nullptr);
  return tier1::mask_func(device, src, binary, dst);
}

} // namespace cle::tier2
