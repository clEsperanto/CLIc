#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"

#include "utils.hpp"

#include <limits>

// Union-find connected component labeling (Playne & Hawick, "A New Algorithm for Parallel Connected-Component
// Labelling on GPUs", IEEE TPDS 2018). The label buffer holds, for every foreground pixel, its parent as a 1-based
// pixel index (0 = background). A root points to itself, and a root is always the smallest pixel index of its
// component: the result before relabeling is identical to the former iterative minimum propagation, but it takes a
// fixed number of kernel launches instead of one launch (and one host read-back) per pixel of the longest path.
//
// The labels are INT32 (all backends map atomic_min to a 32-bit signed atomic), so images are limited to 2^31 - 1 pixels.

namespace kernel
{

// labels = pixel index + 1 for foreground pixels, 0 for background. Computed in integers: a float index (as in
// set_nonzero_pixels_to_pixelindex) is not exact above 2^24 pixels, and the union-find needs exact indices.
constexpr const char * ccl_union_find_init = R"CLC(
__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

__kernel void ccl_union_find_init(
    IMAGE_src_TYPE    src,
    IMAGE_labels_TYPE labels
)
{
  const int x = get_global_id(0);
  const int y = get_global_id(1);
  const int z = get_global_id(2);

  const int w = GET_IMAGE_WIDTH(labels);
  const int h = GET_IMAGE_HEIGHT(labels);

  const int index = (z * h + y) * w + x;
  const IMAGE_src_PIXEL_TYPE value = READ_IMAGE(src, sampler, POS_src_INSTANCE(x,y,z,0)).x;
  labels[index] = (value != 0) ? index + 1 : 0;
}
)CLC";

// Each foreground pixel merges its tree with the trees of its foreground neighbours that come before it in scan
// order (half of the neighbourhood: every pair is merged once). The larger root is attached to the smaller one with
// atomic_min; if another thread changed that root in the meantime, retry from the value it holds.
// Plain reads of the labels are safe: a label only ever decreases and always designates an ancestor.
constexpr const char * ccl_union_find_merge = R"CLC(
__kernel void ccl_union_find_merge(
    IMAGE_labels_TYPE labels,
    const int         diamond
)
{
  const int x = get_global_id(0);
  const int y = get_global_id(1);
  const int z = get_global_id(2);

  const int w = GET_IMAGE_WIDTH(labels);
  const int h = GET_IMAGE_HEIGHT(labels);
  const int d = GET_IMAGE_DEPTH(labels);

  const int self = labels[(z * h + y) * w + x];
  if (self == 0) {
    return;
  }

  const int rx = (w > 1);
  const int ry = (h > 1);
  const int rz = (d > 1);
  for (int dz = -rz; dz <= 0; ++dz) {
    for (int dy = -ry; dy <= ry; ++dy) {
      for (int dx = -rx; dx <= rx; ++dx) {
        if (dz == 0 && (dy > 0 || (dy == 0 && dx >= 0))) {
          continue;
        }
        if (diamond && (abs(dx) + abs(dy) + abs(dz)) != 1) {
          continue;
        }
        const int nx = x + dx;
        const int ny = y + dy;
        const int nz = z + dz;
        if (nx < 0 || nx >= w || ny < 0 || ny >= h || nz < 0) {
          continue;
        }
        int b = labels[(nz * h + ny) * w + nx];
        if (b == 0) {
          continue;
        }
        int a = self;
        while (1) {
          int parent = labels[a - 1];
          while (parent != a) {
            a = parent;
            parent = labels[a - 1];
          }
          parent = labels[b - 1];
          while (parent != b) {
            b = parent;
            parent = labels[b - 1];
          }
          if (a == b) {
            break;
          }
          if (a < b) {
            const int tmp = a;
            a = b;
            b = tmp;
          }
          const int old = atomic_min(&labels[a - 1], b);
          if (old == a) {
            break;
          }
          a = old;
        }
      }
    }
  }
}
)CLC";

// Every foreground pixel points directly to its root.
constexpr const char * ccl_union_find_compress = R"CLC(
__kernel void ccl_union_find_compress(
    IMAGE_labels_TYPE labels
)
{
  const int x = get_global_id(0);
  const int y = get_global_id(1);
  const int z = get_global_id(2);

  const int w = GET_IMAGE_WIDTH(labels);
  const int h = GET_IMAGE_HEIGHT(labels);

  const int index = (z * h + y) * w + x;
  int a = labels[index];
  if (a == 0) {
    return;
  }
  int parent = labels[a - 1];
  while (parent != a) {
    a = parent;
    parent = labels[a - 1];
  }
  labels[index] = a;
}
)CLC";

} // namespace kernel

namespace cle::tier5
{

// Former implementation: iterative minimum propagation, one nonzero_minimum launch and one flag read-back per
// iteration until convergence (as many iterations as the longest path inside a component). Kept for reference.
//
// auto
// connected_component_labeling_func(const Device::Pointer & device,
//                                   const Array::Pointer &  src,
//                                   Array::Pointer          dst,
//                                   const std::string &     connectivity) -> Array::Pointer
// {
//   tier0::create_like(src, dst, dType::LABEL);
//   auto temp1 = tier1::set_nonzero_pixels_to_pixelindex_func(device, src, nullptr, 1);
//   auto temp2 = Array::create(temp1);
//   temp2->fill(0);
//
//   auto flag = Array::create(1, 1, 1, 1, dType::INT32, mType::BUFFER, device);
//   flag->fill(0);
//
//   int flag_value = 1;
//   int iter_count = 0;
//   while (flag_value > 0)
//   {
//     auto active = (iter_count % 2 == 0) ? temp1 : temp2;
//     auto passive = (iter_count % 2 == 0) ? temp2 : temp1;
//     tier1::nonzero_minimum_func(device, active, flag, passive, connectivity);
//     flag->readTo(&flag_value);
//     if (flag_value > 0)
//     {
//       flag->fill(0);
//     }
//     iter_count++;
//   }
//   return tier4::relabel_sequential_func(device, (iter_count % 2 == 0) ? temp1 : temp2, dst, 4096);
// }

auto
connected_component_labeling_func(const Device::Pointer & device,
                                  const Array::Pointer &  src,
                                  Array::Pointer          dst,
                                  const std::string &     connectivity) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::LABEL);
  if (src->size() > static_cast<size_t>(std::numeric_limits<int>::max()))
  {
    throw std::invalid_argument("connected_component_labeling: image too large (more than 2^31 - 1 pixels)");
  }
  // always a buffer: the kernels index the labels directly and use atomics
  auto labels = Array::create(src->width(), src->height(), src->depth(), src->dim(), dType::INT32, mType::BUFFER, device);

  const RangeArray range = { labels->width(), labels->height(), labels->depth() };
  const int        diamond = (connectivity == "sphere") ? 1 : 0;
  execute(device, { "ccl_union_find_init", kernel::ccl_union_find_init }, { { "src", src }, { "labels", labels } }, range);
  execute(device, { "ccl_union_find_merge", kernel::ccl_union_find_merge }, { { "labels", labels }, { "diamond", diamond } }, range);
  execute(device, { "ccl_union_find_compress", kernel::ccl_union_find_compress }, { { "labels", labels } }, range);
  return tier4::relabel_sequential_func(device, labels, dst, 4096);
}

auto
connected_components_labeling_func(const Device::Pointer & device,
                                   const Array::Pointer &  src,
                                   Array::Pointer          dst,
                                   const std::string &     connectivity) -> Array::Pointer
{
  return connected_component_labeling_func(device, src, dst, connectivity);
}

} // namespace cle::tier5
