#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"

#include "utils.hpp"

#include "cle_generate_touch_matrix.h"

namespace cle::tier3
{

auto
generate_touch_matrix_func(const Device::Pointer & device, const Array::Pointer & input_labels, Array::Pointer dst)
  -> Array::Pointer
{
  if (dst == nullptr)
  {
    auto max_label = tier2::maximum_of_all_pixels_func(device, input_labels) + 1;
    tier0::create_dst(input_labels, dst, max_label, max_label, 1, dType::INDEX);
  }
  dst->fill(0);
  const KernelInfo    kernel_code = { "generate_touch_matrix", kernel::generate_touch_matrix };
  const ParameterList params = { { "src", input_labels }, { "dst", dst } };
  const RangeArray    range = { input_labels->width(), input_labels->height(), input_labels->depth() };
  execute(device, kernel_code, params, range);
  return dst;
}

} // namespace cle::tier3
