#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"
#include "tier6.hpp"
#include "tier7.hpp"

#include "utils.hpp"

namespace cle::tier7
{

auto
rigid_transform_func(const Device::Pointer & device,
                     const Array::Pointer &  src,
                     Array::Pointer          dst,
                     float                   translate_x,
                     float                   translate_y,
                     float                   translate_z,
                     float                   angle_x,
                     float                   angle_y,
                     float                   angle_z,
                     bool                    centered,
                     bool                    interpolate,
                     bool                    resize) -> Array::Pointer
{
  auto transform = AffineTransform();
  if (centered)
  {
    transform.center({ src->width(), src->height(), src->depth() }, false);
  }
  if (angle_x != 0)
  {
    transform.rotate(0, angle_x);
  }
  if (angle_y != 0)
  {
    transform.rotate(1, angle_y);
  }
  if (angle_z != 0)
  {
    transform.rotate(2, angle_z);
  }
  if (centered)
  {
    transform.center({ src->width(), src->height(), src->depth() }, true);
  }
  transform.translate(translate_x, translate_y, translate_z);
  return tier0::apply_affine_transform(src, dst, transform, interpolate, resize);
}

} // namespace cle::tier7
