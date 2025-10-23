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
deskew_x_func(const Device::Pointer & device,
              const Array::Pointer &  src,
              Array::Pointer          dst,
              float                   angle,
              float                   voxel_size_x,
              float                   voxel_size_y,
              float                   voxel_size_z,
              float                   scale_factor) -> Array::Pointer
{
  auto transform = AffineTransform();
  transform.deskew_x(angle, voxel_size_x, voxel_size_y, voxel_size_z, scale_factor);

  return apply_affine_transform_deskew_3d(
    src, dst, transform, angle, voxel_size_x, voxel_size_y, voxel_size_z, 0, true);
}


auto
deskew_y_func(const Device::Pointer & device,
              const Array::Pointer &  src,
              Array::Pointer          dst,
              float                   angle,
              float                   voxel_size_x,
              float                   voxel_size_y,
              float                   voxel_size_z,
              float                   scale_factor) -> Array::Pointer
{
  auto transform = AffineTransform();
  transform.deskew_y(angle, voxel_size_x, voxel_size_y, voxel_size_z, scale_factor);

  return apply_affine_transform_deskew_3d(
    src, dst, transform, angle, voxel_size_x, voxel_size_y, voxel_size_z, 1, true);
}

} // namespace cle::tier7
