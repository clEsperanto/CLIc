#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"
#include "tier6.hpp"

#include "utils.hpp"

namespace cle::tier6
{


auto
proximal_neighbor_count_map_func(const Device::Pointer & device,
                                 const Array::Pointer &  labels,
                                 Array::Pointer          dst,
                                 float                   min_distance,
                                 float                   max_distance) -> Array::Pointer
{
  tier0::create_like(labels, dst, dType::FLOAT);

    min_distance = std::max(min_distance, 0.0f);
  max_distance = (max_distance < 0) ? std::numeric_limits<float>::max() : max_distance;

  auto nb_touching_neighbors = tier5::proximal_neighbor_count_func(device, labels, nullptr, max_distance, min_distance);
    tier1::replace_intensities_func(device, labels, nb_touching_neighbors, dst); 
    return dst;
}

} // namespace cle::tier6
