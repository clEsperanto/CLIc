#ifndef __INCLUDE_TIER2_HPP
#define __INCLUDE_TIER2_HPP

#include "array.hpp"
#include "device.hpp"
#include "execution.hpp"


namespace cle::tier2
{

auto
difference_of_gaussian_func(const Device::Pointer & device,
                            const Array::Pointer &  src,
                            Array::Pointer          dst,
                            const float &           sigma1_x,
                            const float &           sigma1_y,
                            const float &           sigma1_z,
                            const float &           sigma2_x,
                            const float &           sigma2_y,
                            const float &           sigma2_z) -> Array::Pointer;

} // namespace cle::tier2

#endif // __INCLUDE_TIER2_HPP
