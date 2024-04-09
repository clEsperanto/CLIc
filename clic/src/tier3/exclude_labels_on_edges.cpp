#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"

#include "utils.hpp"

#include "cle_exclude_on_edges.h"

namespace cle::tier3
{

auto
exclude_labels_on_edges_func(const Device::Pointer & device,
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
  if (exclude_x && src->width() > 1)
  {
    const KernelInfo kernel = { "exclude_on_edges_x", kernel::exclude_on_edges };
    const RangeArray range = { 1, src->height(), src->depth() };
    execute(device, kernel, params, range);
  }
  if (exclude_y && src->height() > 1)
  {
    const KernelInfo kernel = { "exclude_on_edges_y", kernel::exclude_on_edges };
    const RangeArray range = { src->width(), 1, src->depth() };
    execute(device, kernel, params, range);
  }
  if (exclude_z && src->depth() > 1)
  {
    const KernelInfo kernel = { "exclude_on_edges_z", kernel::exclude_on_edges };
    const RangeArray range = { src->width(), src->height(), 1 };
    execute(device, kernel, params, range);
  }
  std::vector<uint32_t> label_map_vector(label_map->size());
  label_map->read(label_map_vector.data());
  int32_t count = 1;
  for (auto & i : label_map_vector)
  {
    if (i > 0)
    {
      i = count;
      count++;
    }
  }
  label_map->write(label_map_vector.data());
  return tier1::replace_values_func(device, src, label_map, dst);
}

} // namespace cle::tier3
