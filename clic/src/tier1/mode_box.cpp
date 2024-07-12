#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_mode_box.h"

namespace cle::tier1
{

auto
mode_box_func(const Device::Pointer & device,
              const Array::Pointer &  src,
              Array::Pointer          dst,
              int                     radius_x,
              int                     radius_y,
              int                     radius_z) -> Array::Pointer
{
  std::cerr << "Deprecated: Please use mode() instead\n";
  if (src->dtype() != dType::UINT8)
  {
    std::cerr << "Warning: mode only support uint8 pixel input type." << std::endl;
  }
  tier0::create_like(src, dst, dType::UINT8);
  const KernelInfo    kernel = { "mode_box", kernel::mode_box };
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
