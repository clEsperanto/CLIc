
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
                                 const std::vector<int> & dilation_radii,
                                const bool & include_background) -> StatisticsMap
{
  auto nei_stats = compute_neighbors_statistics_per_labels(device, label, proximal_distances, nearest_neighbor_ns, dilation_radii);

  if (!include_background)
  {
    // remove background label statistics (index 0) from all exported vectors
    for (auto & entry : nei_stats)
    {
      auto & vec = entry.second;
      if (!vec.empty())
      {
        vec.erase(vec.begin());
      }
    }
  }
  else
  {
    // set background label statistics (index 0) to 0 for all exported vectors
    for (auto & entry : nei_stats)
    {
      auto & vec = entry.second;
      if (!vec.empty())
      {
        vec[0] = 0.0f;
      }
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
  return labels_neighbors_statistics_func(device, label, proximal_distances, nearest_neighbor_ns, dilation_radii, false);
}


} // namespace cle::tier7
