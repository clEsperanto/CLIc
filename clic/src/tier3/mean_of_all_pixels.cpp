#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"

#include "utils.hpp"

namespace cle::tier3
{

auto
mean_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float
{
  auto temp = tier2::sum_of_all_pixels_func(device, src);
  return temp / src->size();
}

} // namespace cle::tier3
