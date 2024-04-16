#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_mean_separable.h"

namespace cle::tier1
{

auto
mean_box_func(const Device::Pointer & device,
              const Array::Pointer &  src,
              Array::Pointer          dst,
              int                     radius_x,
              int                     radius_y,
              int                     radius_z) -> Array::Pointer
{
  std::cerr << "Deprecated: Please use mean() instead\n";
  tier0::create_like(src, dst);
  const KernelInfo kernel = { "mean_separable", kernel::mean_separable };
  execute_separable(device,
                    kernel,
                    src,
                    dst,
                    { static_cast<float>(radius_x), static_cast<float>(radius_y), static_cast<float>(radius_z) },
                    { radius2kernelsize(radius_x), radius2kernelsize(radius_y), radius2kernelsize(radius_z) });
  return dst;
}

} // namespace cle::tier1
