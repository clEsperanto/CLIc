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
reduce_labels_to_centroids_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::LABEL);
  dst->fill(0);

  auto pos = tier4::centroids_of_labels_func(device, src, nullptr, true);
  auto label_pos = Array::create(pos->width(), 4, 1, 2, dType::FLOAT, mType::BUFFER, device);
  tier1::set_ramp_x_func(device, label_pos);
  pos->copyTo(label_pos, { pos->width(), 3, 1 }, { 0, 0, 0 }, { 0, 0, 0 });

  tier1::set_column_func(device, label_pos, 0, -1);
  tier1::nan_to_num_func(device, label_pos, label_pos, -1, -1, -1);
  return tier1::write_values_to_positions_func(device, label_pos, dst);
}

} // namespace cle::tier5
