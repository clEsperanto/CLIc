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


/**
 * @brief Compute a list of neighborhood statistics for each label in the label image
 * The output is a map with the following keys:
 * The vector length is equal to the number of labels in the label image.
 *
 * @param device Device::Pointer to use
 * @param label Array::Pointer of the label image
 * @param proximal_distances std::vector<int> of the proximal distances
 * @param nearest_neighbor_ns std::vector<int> of the nearest neighbor numbers
 * @param dilation_radii std::vector<int> of the dilation radii
 * @return StatisticsMap
 */
auto
compute_statistics_per_labels(const Device::Pointer &  device,
                              const Array::Pointer &   label,
                              const std::vector<int> & proximal_distances,
                              const std::vector<int> & nearest_neighbor_ns,
                              const std::vector<int> & dilation_radii) -> StatisticsMap;


/**
 * @brief Compute a list of neighborhood statistics for each label in the label image
 * The output is a map with the following keys:
 * The vector length is equal to the number of labels in the label image.
 *
 * This function is similar to compute_statistics_per_labels() but excludes the background label (0) from the output statistics
 * and compute statistics on the nearest neighbor (distance, number of neighbor, how many pixels they share on the boundary, etc.) instead
 * of the label itself.
 *
 * @param device Device::Pointer to use
 * @param label Array::Pointer of the label image
 * @param proximal_distances std::vector<int> of the proximal distances
 * @param nearest_neighbor_ns std::vector<int> of the nearest neighbor numbers
 * @param dilation_radii std::vector<int> of the dilation radii
 * @return StatisticsMap
 *
 */
auto
compute_neighbors_statistics_per_labels(const Device::Pointer &  device,
                                        const Array::Pointer &   label,
                                        const std::vector<int> & proximal_distances,
                                        const std::vector<int> & nearest_neighbor_ns,
                                        const std::vector<int> & dilation_radii) -> StatisticsMap;

} // namespace cle
