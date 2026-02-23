#pragma once

#include "array.hpp"

#include <unordered_map>
#include <vector>

namespace cle
{

using StatisticsMap = std::unordered_map<std::string, std::vector<float>>;

/**
 * @brief Compute a list of statistics for each label in the label image (mean, std, min, max)
 * using the intensity image and label image as input.
 * The output is a map with the following keys: "mean", "std", "min", "max" etc. and a vector of float values
 * associated with each statistics.
 * The vector length is equal to the number of labels in the label image.
 *
 * @param device Device::Pointer to use
 * @param label Array::Pointer of the label image
 * @param intensity Array::Pointer of the intensity image
 * @return StatisticsMap
 */
auto
compute_statistics_per_labels(const Device::Pointer & device, const Array::Pointer & label, const Array::Pointer & intensity)
  -> StatisticsMap;

/**
 * @brief Subfunction to compute statistics per label
 */
auto
_statistics_per_label(const Device::Pointer & device, const Array::Pointer & label, const Array::Pointer & intensity, int nb_labels)
  -> Array::Pointer;

/**
 * @brief Subfunction to compute standard deviation per label
 */
auto
_std_per_label(const Device::Pointer & device,
               const Array::Pointer &  statistics,
               const Array::Pointer &  label,
               const Array::Pointer &  intensity,
               int                     nb_labels) -> Array::Pointer;

} // namespace cle

