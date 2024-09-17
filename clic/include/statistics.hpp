#ifndef __INCLUDE_STATISTICS_HPP
#define __INCLUDE_STATISTICS_HPP

#include "array.hpp"

#include <unordered_map>
#include <vector>

namespace cle
{

using StatisticsMap = std::unordered_map<std::string, std::vector<float>>;

auto
compute_statistics_per_labels(const Device::Pointer & device,
                              const Array::Pointer &  label,
                              const Array::Pointer &  intensity) -> StatisticsMap;

auto
_statistics_per_label(const Device::Pointer & device,
                      const Array::Pointer &  label,
                      const Array::Pointer &  intensity,
                      int                     nb_labels) -> Array::Pointer;

auto
_std_per_label(const Device::Pointer & device,
               const Array::Pointer &  statistics,
               const Array::Pointer &  label,
               const Array::Pointer &  intensity,
               int                     nb_labels) -> Array::Pointer;

} // namespace cle

#endif // __INCLUDE_STATISTICS_HPP
