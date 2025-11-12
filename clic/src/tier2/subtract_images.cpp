#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

namespace cle::tier2
{

auto
subtract_images_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  return tier1::add_images_weighted_func(device, src0, src1, dst, 1, -1);
}

} // namespace cle::tier2
