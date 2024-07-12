#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_replace_value.h"
#include "cle_replace_values.h"

namespace cle::tier1
{

auto
replace_value_func(const Device::Pointer & device,
                   const Array::Pointer &  src,
                   Array::Pointer          dst,
                   float                   scalar0,
                   float                   scalar1) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "replace_value", kernel::replace_value };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar0", scalar0 }, { "scalar1", scalar1 } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

// resample_func
// touch_matrix_to_mesh_func

} // namespace cle::tier1
