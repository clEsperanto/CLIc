#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_paste.h"

namespace cle::tier1
{

auto
paste_func(const Device::Pointer & device,
           const Array::Pointer &  src,
           Array::Pointer          dst,
           int                     destination_x,
           int                     destination_y,
           int                     destination_z) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "paste", kernel::paste };
  const ParameterList params = {
    { "src", src }, { "dst", dst }, { "scalar0", destination_x }, { "scalar1", destination_y }, { "scalar2", destination_z }
  };
  const RangeArray range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
