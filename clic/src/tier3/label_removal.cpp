#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "utils.hpp"

#include "cle_exclude_on_edges.h"

namespace
{

// replace_values with a bounds check: a label past the end of the lookup table has no flag, it is kept and numbered
// after the ones of the table (label - shift), as if the table had been padded with zeros up to the largest label.
constexpr const char * remove_labels_lookup_kernel = R"CLC(
  __constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

  __kernel void remove_labels_lookup(
      IMAGE_src_TYPE    src,
      IMAGE_lookup_TYPE lookup,
      IMAGE_dst_TYPE    dst,
      int               lookup_size,
      int               shift
  )
  {
    const int x = get_global_id(0);
    const int y = get_global_id(1);
    const int z = get_global_id(2);

    const int label = (int) READ_IMAGE(src, sampler, POS_src_INSTANCE(x, y, z, 0)).x;
    const int value = (label < lookup_size) ? (int) READ_IMAGE(lookup, sampler, POS_lookup_INSTANCE(label, 0, 0, 0)).x : label - shift;
    WRITE_IMAGE(dst, POS_dst_INSTANCE(x, y, z, 0), CONVERT_dst_PIXEL_TYPE(value));
  }
)CLC";

} // namespace

namespace cle::tier3
{

auto
remove_labels_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & list, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  if (list->dtype() != dType::LABEL)
  {
    throw std::runtime_error("remove_labels: label list must be of type uint32");
  }
  std::vector<uint32_t> labels_list(list->size());
  list->readTo(labels_list.data());

  labels_list.front() = 0;
  uint32_t count = 1;
  for (size_t i = 1; i < labels_list.size(); i++)
  {
    if (labels_list[i] == 0)
    {
      labels_list[i] = count;
      count++;
    }
    else
    {
      labels_list[i] = 0;
    }
  }

  auto index_list = Array::create(labels_list.size(), 1, 1, 1, dType::LABEL, mType::BUFFER, src->device());
  index_list->writeFrom(labels_list.data());

  // labels from labels_list.size() on are numbered from `count` on
  const int           lookup_size = static_cast<int>(labels_list.size());
  const KernelInfo    kernel = { "remove_labels_lookup", remove_labels_lookup_kernel };
  const ParameterList params = { { "src", src },
                                 { "lookup", index_list },
                                 { "dst", dst },
                                 { "lookup_size", lookup_size },
                                 { "shift", lookup_size - static_cast<int>(count) } };
  execute(device, kernel, params, { dst->width(), dst->height(), dst->depth() });
  return dst;
}

auto
exclude_labels_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & list, Array::Pointer dst)
  -> Array::Pointer
{
  return remove_labels_func(device, src, list, dst);
}

auto
remove_labels_on_edges_func(const Device::Pointer & device,
                            const Array::Pointer &  src,
                            Array::Pointer          dst,
                            bool                    exclude_x,
                            bool                    exclude_y,
                            bool                    exclude_z) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::LABEL);
  auto num_labels = static_cast<uint32_t>(tier1::maximum_of_all_pixels_func(device, src));
  auto label_map = Array::create(num_labels + 1, 1, 1, 1, dType::LABEL, mType::BUFFER, src->device());
  tier1::set_ramp_x_func(device, label_map);

  const ParameterList params = { { "src", src }, { "dst", label_map } };

  auto execute_if_needed = [&](bool exclude, int dimension, int axis) {
    if (exclude && dimension > 1)
    {
      const KernelInfo   kernel = { "exclude_on_edges", kernel::exclude_on_edges };
      const RangeArray   range = { (dimension == 0) ? 1 : src->width(),
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
