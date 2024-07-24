#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_median_box.h"

namespace cle::tier1
{

auto
median_box_func(const Device::Pointer & device,
                const Array::Pointer &  src,
                Array::Pointer          dst,
                int                     radius_x,
                int                     radius_y,
                int                     radius_z) -> Array::Pointer
{
  std::cerr << "Deprecated: Please use median() instead\n";
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "median_box", kernel::median_box };
  const ParameterList params = { { "src", src },
                                 { "dst", dst },
                                 { "scalar0", radius2kernelsize(radius_x) },
                                 { "scalar1", radius2kernelsize(radius_y) },
                                 { "scalar2", radius2kernelsize(radius_z) } };
  const ConstantList  constants = {
    { "MAX_ARRAY_SIZE", radius2kernelsize(radius_x) * radius2kernelsize(radius_y) * radius2kernelsize(radius_z) }
  };
  const RangeArray range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range, constants);
  return dst;
}

} // namespace cle::tier1
