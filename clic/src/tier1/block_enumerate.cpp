#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_block_enumerate.h"

namespace cle::tier1
{

auto
block_enumerate_func(const Device::Pointer & device,
                     const Array::Pointer &  src0,
                     const Array::Pointer &  src1,
                     Array::Pointer          dst,
                     int                     blocksize) -> Array::Pointer
{
  tier0::create_like(src0, dst, dType::FLOAT);
  const KernelInfo    kernel = { "block_enumerate", kernel::block_enumerate };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst }, { "index", blocksize } };
  const RangeArray    range = { src1->width(), src1->height(), src1->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
