#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_detect_label_edges.h"

namespace cle::tier1
{

auto
detect_label_edges_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::BINARY);
  const KernelInfo    kernel_code = { "detect_label_edges", kernel::detect_label_edges };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel_code, params, range);
  return dst;
}

} // namespace cle::tier1
