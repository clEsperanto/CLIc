#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"

#include "utils.hpp"

namespace cle::tier4
{

auto
label_bounding_box_func(const Device::Pointer & device, const Array::Pointer & src, int label_id) -> std::vector<float>
{
  auto binary = tier1::equal_constant_func(device, src, nullptr, label_id);
  return tier3::bounding_box_func(device, binary);
}

} // namespace cle::tier4
