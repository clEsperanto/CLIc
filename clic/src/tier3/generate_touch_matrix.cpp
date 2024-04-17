#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"

#include "utils.hpp"

#include "cle_generate_touch_matrix.h"

namespace cle::tier3
{

auto
generate_touch_matrix_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  if (dst == nullptr)
  {
    auto max_label = tier2::maximum_of_all_pixels_func(device, src) + 1;
    tier0::create_dst(src, dst, max_label, max_label, 1, dType::INDEX);
  }
  dst->fill(0);
  const KernelInfo    kernel = { "generate_touch_matrix", kernel::generate_touch_matrix };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier3
