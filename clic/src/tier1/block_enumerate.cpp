#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_block_enumerate.h"

namespace cle::tier1
{

auto
block_enumerate_func(const Device::Pointer & device,
                     const Array::Pointer &  src,
                     const Array::Pointer &  sums,
                     Array::Pointer          dst,
                     int                     blocksize) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  const KernelInfo    kernel_code = { "block_enumerate", kernel::block_enumerate };
  const ParameterList params = { { "src0", src }, { "src1", sums }, { "dst", dst }, { "index", blocksize } };
  const RangeArray    range = { sums->width(), sums->height(), sums->depth() };
  execute(device, kernel_code, params, range);
  return dst;
}

} // namespace cle::tier1
