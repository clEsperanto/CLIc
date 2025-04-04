#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_set.h"
#include "cle_set_column.h"
#include "cle_set_image_borders.h"
#include "cle_set_nonzero_pixels_to_pixelindex.h"
#include "cle_set_plane.h"
#include "cle_set_ramp_x.h"
#include "cle_set_ramp_y.h"
#include "cle_set_ramp_z.h"
#include "cle_set_row.h"
#include "cle_set_where_x_equals_y.h"
#include "cle_set_where_x_greater_than_y.h"
#include "cle_set_where_x_smaller_than_y.h"

namespace cle::tier1
{

auto
set_func(const Device::Pointer & device, const Array::Pointer & src, float scalar) -> Array::Pointer
{
  const KernelInfo    kernel_code = { "set", kernel::set };
  const ParameterList params = { { "dst", src }, { "scalar", scalar } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel_code, params, range);
  return src;
}

} // namespace cle::tier1
