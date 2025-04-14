#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_copy.h"
#include "cle_copy_horizontal_slice_from.h"
#include "cle_copy_horizontal_slice_to.h"
#include "cle_copy_slice_from.h"
#include "cle_copy_slice_to.h"
#include "cle_copy_vertical_slice_from.h"
#include "cle_copy_vertical_slice_to.h"

namespace cle::tier1
{

auto
copy_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel_code = { "copy", kernel::copy };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel_code, params, range);
  return dst;
}

} // namespace cle::tier1
