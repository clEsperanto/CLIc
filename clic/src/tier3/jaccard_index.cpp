#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"

#include "utils.hpp"

namespace cle::tier3
{

auto
jaccard_index_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1) -> float
{
  auto intersection_ = tier1::binary_and_func(device, src0, src1, nullptr);
  auto union_ = tier1::binary_or_func(device, src0, src1, nullptr);
  return tier2::sum_of_all_pixels_func(device, intersection_) / tier2::sum_of_all_pixels_func(device, union_);
}

} // namespace cle::tier3
