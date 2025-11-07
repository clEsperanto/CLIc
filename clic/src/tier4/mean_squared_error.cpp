#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"

#include "utils.hpp"

namespace cle::tier4
{

auto
mean_squared_error_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1) -> float
{
  auto temp = tier2::squared_difference_func(device, src0, src1, nullptr);
  return tier3::mean_of_all_pixels_func(device, temp);
}

} // namespace cle::tier4
