#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"

#include "utils.hpp"

namespace cle::tier5
{

auto
proximal_neighbor_count_func(const Device::Pointer & device,
                             const Array::Pointer &  src,
                             Array::Pointer          dst,
                             float                   min_distance,
                             float                   max_distance) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);

  min_distance = std::max(min_distance, 0.0f);
  max_distance = (max_distance < 0) ? std::numeric_limits<float>::max() : max_distance;

  auto pointlist = tier4::centroids_of_labels_func(device,src, nullptr, false);
  auto distance_matrix = tier1::generate_distance_matrix_func(device,pointlist, pointlist, nullptr);
  auto touch_matrix =
    tier2::generate_proximal_neighbors_matrix_func(device,distance_matrix, nullptr, min_distance, max_distance);

  tier2::count_touching_neighbors_func(device, touch_matrix, dst, false);
  tier1::set_column_func(device, dst, 0, 0);

  return dst;
}

} // namespace cle::tier5
