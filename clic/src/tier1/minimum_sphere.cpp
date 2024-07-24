#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_minimum_sphere.h"

namespace cle::tier1
{

auto
minimum_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z) -> Array::Pointer
{
  std::cerr << "Deprecated: Please use minimum() instead\n";
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "minimum_sphere", kernel::minimum_sphere };
  const ParameterList params = { { "src", src },
                                 { "dst", dst },
                                 { "scalar0", radius2kernelsize(radius_x) },
                                 { "scalar1", radius2kernelsize(radius_y) },
                                 { "scalar2", radius2kernelsize(radius_z) } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
