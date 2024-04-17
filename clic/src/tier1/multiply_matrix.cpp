#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_multiply_matrix.h"

namespace cle::tier1
{

auto
multiply_matrix_func(const Device::Pointer & device,
                     const Array::Pointer &  src0,
                     const Array::Pointer &  src1,
                     Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_dst(src0, dst, src1->width(), src0->height(), src0->depth(), dType::FLOAT);
  const KernelInfo    kernel = { "multiply_matrix", kernel::multiply_matrix };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
