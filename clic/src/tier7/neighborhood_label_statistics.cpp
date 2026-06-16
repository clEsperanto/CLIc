
#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"
#include "tier6.hpp"
#include "tier7.hpp"

#include "transform.hpp"
#include "utils.hpp"

namespace cle::tier7
{

auto
labels_neighbors_statistics_func(const Device::Pointer &  device,
                                 const Array::Pointer     label,
                                 const std::vector<int> & proximal_distances,
                                 const std::vector<int> & nearest_neighbor_ns,
                                 const std::vector<int> & dilation_radii) -> StatisticsMap
{
  auto nei_stats = compute_neighbors_statistics_per_labels(device, label, proximal_distances, nearest_neighbor_ns, dilation_radii);

  // remove background label statistics (index 0) from all exported vectors
  for (auto & entry : nei_stats)
  {
    auto & vec = entry.second;
    if (!vec.empty())
    {
      vec.erase(vec.begin());
    }
  }

  return nei_stats;
}

auto
statistics_of_labelled_neighbors_func(const Device::Pointer &  device,
                                      const Array::Pointer     label,
                                      const std::vector<int> & proximal_distances,
                                      const std::vector<int> & nearest_neighbor_ns,
                                      const std::vector<int> & dilation_radii) -> StatisticsMap
{
  return labels_neighbors_statistics_func(device, label, proximal_distances, nearest_neighbor_ns, dilation_radii);
}


}
