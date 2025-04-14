#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"

#include "utils.hpp"

#include "cle_generate_binary_overlap_matrix.h"

namespace cle::tier3
{

auto
generate_binary_overlap_matrix_func(const Device::Pointer & device,
                                    const Array::Pointer &  src0,
                                    const Array::Pointer &  src1,
                                    Array::Pointer          dst) -> Array::Pointer
{
  if (dst == nullptr)
  {
    auto max_label_0 = tier2::maximum_of_all_pixels_func(device, src0) + 1;
    auto max_label_1 = tier2::maximum_of_all_pixels_func(device, src1) + 1;
    tier0::create_dst(src0, dst, max_label_0, max_label_1, 1, dType::INDEX);
  }
  dst->fill(0);
  const KernelInfo    kernel_code = { "generate_binary_overlap_matrix", kernel::generate_binary_overlap_matrix };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { src0->width(), src0->height(), src0->depth() };
  execute(device, kernel_code, params, range);
  return dst;
}

} // namespace cle::tier3
