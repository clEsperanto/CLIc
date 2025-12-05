#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"

#include "utils.hpp"

#include "cle_exclude_on_edges.h"

namespace cle::tier3
{

auto
remove_labels_on_edges_func(const Device::Pointer & device,
                            const Array::Pointer &  src,
                            Array::Pointer          dst,
                            bool                    exclude_x,
                            bool                    exclude_y,
                            bool                    exclude_z) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::LABEL);
  auto num_labels = static_cast<uint32_t>(tier2::maximum_of_all_pixels_func(device, src));
  auto label_map = Array::create(num_labels + 1, 1, 1, 1, dType::LABEL, mType::BUFFER, src->device());
  tier1::set_ramp_x_func(device, label_map);

  const ParameterList params = { { "src", src }, { "dst", label_map } };

  auto execute_if_needed = [&](bool exclude, int dimension, int axis) {
    if (exclude && dimension > 1)
    {
      const KernelInfo kernel = { "exclude_on_edges", kernel::exclude_on_edges };
      const RangeArray range = { (dimension == 0) ? 1 : src->width(),
                                 (dimension == 1) ? 1 : src->height(),
                                 (dimension == 2) ? 1 : src->depth() };
      const RangeArray   local = { 1, 1, 1 };
      const ConstantList constants = { { "EXCLUDE_AXIS", axis } }; // 0 for X axis, 1 for Y axis, 2 for Z axis
      execute(device, kernel, params, range, local, constants);
    }
  };

  execute_if_needed(exclude_x, src->width(), 0);
  execute_if_needed(exclude_y, src->height(), 1);
  execute_if_needed(exclude_z, src->depth(), 2);

  std::vector<uint32_t> label_map_vector(label_map->size());
  label_map->readTo(label_map_vector.data());
  int32_t count = 1;
  for (auto & i : label_map_vector)
  {
    if (i > 0)
    {
      i = count;
      count++;
    }
  }
  label_map->writeFrom(label_map_vector.data());
  return tier1::replace_values_func(device, src, label_map, dst);
}

auto
exclude_labels_on_edges_func(const Device::Pointer & device,
                             const Array::Pointer &  src,
                             Array::Pointer          dst,
                             bool                    exclude_x,
                             bool                    exclude_y,
                             bool                    exclude_z) -> Array::Pointer
{
  return remove_labels_on_edges_func(device, src, dst, exclude_x, exclude_y, exclude_z);
}

} // namespace cle::tier3
