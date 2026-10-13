#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

namespace kernel
{

// Subfield-based directional thinning (Nemeth, Kardos & Palagyi, 2010), with the (26,6) simple point test.
// Each launch processes one direction (border voxels whose face neighbour in that direction is background) and one
// subfield (voxels of one (x,y,z) parity). Two voxels of the same subfield are never 26-adjacent, so deleting all simple
// voxels of a subfield at once is equivalent to deleting them one by one: the topology is preserved, and the image can
// be updated in place.
//
// The 3x3x3 neighbourhood is packed in 27 bits, bit = (dx+1) + 3*(dy+1) + 9*(dz+1) (center = bit 13). Connected
// components are counted by flood filling with masked bit shifts. Voxels outside the image are background.
// A 2D image is a single z-plane: the (26,6) test restricted to a plane is the (8,4) test.
constexpr const char * skeletonize = R"CLC(
__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

inline uint dilate_6(const uint m)
{
  return m | ((m << 1) & 0x6DB6DB6u) | ((m >> 1) & 0x36DB6DBu)
           | ((m << 3) & 0x7E3F1F8u) | ((m >> 3) & 0x0FC7E3Fu)
           | ((m << 9) & 0x7FFFFFFu) | (m >> 9);
}

inline uint dilate_26(const uint m)
{
  const uint a = m | ((m << 1) & 0x6DB6DB6u) | ((m >> 1) & 0x36DB6DBu);
  const uint b = a | ((a << 3) & 0x7E3F1F8u) | ((a >> 3) & 0x0FC7E3Fu);
  return b | ((b << 9) & 0x7FFFFFFu) | (b >> 9);
}

// number of connected components of 'set' that contain at least one bit of 'seeds' (stops counting at 2)
inline int count_components(const uint set, uint seeds, const int use_26)
{
  int count = 0;
  while (seeds != 0u && count < 2) {
    uint component = seeds & (~seeds + 1u);
    uint previous = 0u;
    while (component != previous) {
      previous = component;
      component = (use_26 ? dilate_26(component) : dilate_6(component)) & set;
    }
    seeds &= ~component;
    count++;
  }
  return count;
}

// (26,6) simple point: the object neighbours form one 26-component, and the background of the 18-neighbourhood
// has exactly one 6-component that is 6-adjacent to the center
inline int is_simple_point(const uint object)
{
  if (count_components(object, object, 1) != 1) {
    return 0;
  }
  const uint background = ~object & 0x2EBDEBAu;
  return count_components(background, background & 0x0415410u, 0) == 1;
}

__kernel void skeletonize(
    IMAGE_binary_TYPE binary,
    IMAGE_flag_TYPE   flag,
    const int         offset_x,
    const int         offset_y,
    const int         offset_z,
    const int         direction,
    const int         pass
)
{
  const int x = get_global_id(0) * 2 + offset_x;
  const int y = get_global_id(1) * 2 + offset_y;
  const int z = get_global_id(2) * 2 + offset_z;

  const int width  = GET_IMAGE_WIDTH(binary);
  const int height = GET_IMAGE_HEIGHT(binary);
  const int depth  = GET_IMAGE_DEPTH(binary);

  if (x >= width || y >= height || z >= depth) {
    return;
  }
  // 1: object, 0: background, 2..255: deleted during that pass (an object at the start of the current pass if == pass)
  if ((int)READ_IMAGE(binary, sampler, POS_binary_INSTANCE(x, y, z, 0)).x != 1) {
    return;
  }

  // border voxel of the current direction: its face neighbour in that direction is background
  const int bx = x + (direction == 0 ? -1 : (direction == 1 ? 1 : 0));
  const int by = y + (direction == 2 ? -1 : (direction == 3 ? 1 : 0));
  const int bz = z + (direction == 4 ? -1 : (direction == 5 ? 1 : 0));
  if (bx >= 0 && bx < width && by >= 0 && by < height && bz >= 0 && bz < depth) {
    const int value = (int)READ_IMAGE(binary, sampler, POS_binary_INSTANCE(bx, by, bz, 0)).x;
    if (value == 1 || value == pass) {
      return;
    }
  }

  uint before = 0u;  // neighbourhood at the start of the pass
  uint current = 0u; // neighbourhood after the previous subfields of the pass
  int  count = 0;
  for (int dz = -1; dz <= 1; dz++) {
    const int nz = z + dz;
    if (nz < 0 || nz >= depth) {
      continue;
    }
    for (int dy = -1; dy <= 1; dy++) {
      const int ny = y + dy;
      if (ny < 0 || ny >= height) {
        continue;
      }
      for (int dx = -1; dx <= 1; dx++) {
        const int nx = x + dx;
        if (nx < 0 || nx >= width || (dx == 0 && dy == 0 && dz == 0)) {
          continue;
        }
        const int  value = (int)READ_IMAGE(binary, sampler, POS_binary_INSTANCE(nx, ny, nz, 0)).x;
        const uint bit = 1u << ((dx + 1) + 3 * (dy + 1) + 9 * (dz + 1));
        if (value == 1) {
          before |= bit;
          current |= bit;
          count++;
        }
        else if (value == pass) {
          before |= bit;
          count++;
        }
      }
    }
  }

  // As in Lee et al. (1994): candidates are the simple border voxels that are not end points (a single object
  // neighbour) at the start of the pass; a candidate is deleted if it is still simple. Protecting only the end points
  // of the start of the pass avoids freezing the tips created during the pass, which would grow spurious branches.
  if (count <= 1 || !is_simple_point(before)) {
    return;
  }
  if (current != before && !is_simple_point(current)) {
    return;
  }
  WRITE_IMAGE(binary, POS_binary_INSTANCE(x, y, z, 0), CONVERT_binary_PIXEL_TYPE(pass));
  WRITE_IMAGE(flag, POS_flag_INSTANCE(0, 0, 0, 0), CONVERT_flag_PIXEL_TYPE(1));
}
)CLC";

} // namespace kernel

namespace cle::tier2
{

auto
skeletonize_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::BOOL);

  // the thinning runs in place on a binary buffer: an IMAGE cannot be read and written by the same kernel
  auto binary = Array::create(src->width(), src->height(), src->depth(), src->dimension(), dType::BOOL, mType::BUFFER, device);
  tier1::not_equal_constant_func(device, src, binary, 0);
  auto flag = Array::create(1, 1, 1, 1, dType::INT32, mType::BUFFER, device);

  // A voxel deleted during a pass is marked with the pass number (2..255) instead of 0, so the image at the start of the
  // pass (value 1 or pass) is known without copying it. The marks are cleared before the numbers are reused.
  const KernelInfo kernel = { "skeletonize", kernel::skeletonize };
  const int        num_directions = (src->depth() > 1) ? 6 : 4;
  const int        num_offsets_z = (src->depth() > 1) ? 2 : 1;
  int              pass = 1;
  int              flag_value = 1;
  while (flag_value != 0)
  {
    flag->fill(0);
    for (int direction = 0; direction < num_directions; direction++)
    {
      if (++pass > 255)
      {
        binary = tier1::equal_constant_func(device, binary, nullptr, 1);
        pass = 2;
      }
      for (int offset_z = 0; offset_z < num_offsets_z; offset_z++)
      {
        for (int offset_y = 0; offset_y < 2; offset_y++)
        {
          for (int offset_x = 0; offset_x < 2; offset_x++)
          {
            // one work item per voxel of the subfield
            const RangeArray range = { (src->width() + 1 - offset_x) / 2,
                                       (src->height() + 1 - offset_y) / 2,
                                       (src->depth() + 1 - offset_z) / 2 };
            if (range[0] == 0 || range[1] == 0 || range[2] == 0)
            {
              continue;
            }
            const ParameterList params = { { "binary", binary },     { "flag", flag },         { "offset_x", offset_x },
                                           { "offset_y", offset_y }, { "offset_z", offset_z }, { "direction", direction },
                                           { "pass", pass } };
            execute(device, kernel, params, range);
          }
        }
      }
    }
    flag->readTo(&flag_value);
  }
  return tier1::equal_constant_func(device, binary, dst, 1);
}

} // namespace cle::tier2
