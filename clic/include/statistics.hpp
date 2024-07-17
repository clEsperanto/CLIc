#ifndef __INCLUDE_STATISTICS_HPP
#define __INCLUDE_STATISTICS_HPP

#include <numeric>

#include "array.hpp"

namespace cle
{


auto
statistics_of_labelled_pixels(const Device::Pointer & device,
                              Array::Pointer &        intensity,
                              Array::Pointer &        label,
                              Array::Pointer          result) -> std::unordered_map<std::string, std::vector<float>>;

} // namespace cle

#endif // __INCLUDE_STATISTICS_HPP
