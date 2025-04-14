#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_divide_scalar_by_image.h"

namespace cle::tier1
{

auto
divide_scalar_by_image_func(const Device::Pointer & device,
                            const Array::Pointer &  src,
                            Array::Pointer          dst,
                            float                   scalar) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel_code = { "divide_scalar_by_image", kernel::divide_scalar_by_image };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar", scalar } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel_code, params, range);
  return dst;
}

// draw_box_func
// draw_sphere_func
// draw_line_func
// downsample_slice_by_slice_half_median_func

} // namespace cle::tier1
