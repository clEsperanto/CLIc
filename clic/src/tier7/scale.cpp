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
scale_func(const Device::Pointer & device,
           const Array::Pointer &  src,
           Array::Pointer          dst,
           float                   factor_x,
           float                   factor_y,
           float                   factor_z,
           bool                    centered,
           bool                    interpolate,
           bool                    resize) -> Array::Pointer
{
  auto transform = AffineTransform();
  if (centered && !resize)
  {
    transform.center({ src->width(), src->height(), src->depth() }, false);
  }
  transform.scale(factor_x, factor_y, factor_z);
  if (centered && !resize)
  {
    transform.center({ src->width(), src->height(), src->depth() }, true);
  }
  return tier0::apply_affine_transform(src, dst, transform, interpolate, resize);
}

} // namespace cle::tier7
