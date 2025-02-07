#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_nonzero_maximum_box.h"
#include "cle_nonzero_maximum_diamond.h"

namespace cle::tier1
{

auto
nonzero_maximum_func(const Device::Pointer & device,
                     const Array::Pointer &  src,
                     Array::Pointer          flag,
                     Array::Pointer          dst,
                     std::string             connectivity) -> Array::Pointer

{
  tier0::create_like(src, dst);
  KernelInfo kernel_code = { "nonzero_maximum_box", kernel::nonzero_maximum_box };
  if (connectivity == "sphere")
  {
    kernel_code = { "nonzero_maximum_diamond", kernel::nonzero_maximum_diamond };
  }
  const ParameterList params = { { "src", src }, { "dst0", flag }, { "dst1", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel_code, params, range);
  return dst;
}

auto
nonzero_maximum_diamond_func(const Device::Pointer & device,
                             const Array::Pointer &  src,
                             Array::Pointer          flag,
                             Array::Pointer          dst) -> Array::Pointer
{
  return nonzero_maximum_func(device, src, flag, dst, "sphere");
}

auto
nonzero_maximum_box_func(const Device::Pointer & device,
                         const Array::Pointer &  src,
                         Array::Pointer          flag,
                         Array::Pointer          dst) -> Array::Pointer
{
  return nonzero_maximum_func(device, src, flag, dst, "box");
}

} // namespace cle::tier1
